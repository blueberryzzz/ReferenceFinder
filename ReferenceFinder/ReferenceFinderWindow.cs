using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
using UnityEditor.IMGUI.Controls;
using System.Runtime.InteropServices;
using System;
using static UnityEngine.Rendering.DebugUI;
using static UnityEditor.Progress;
using System.Globalization;
using System.Linq;
using System.Text;
using static ReferenceFinderData;
public class ReferenceFinderWindow : EditorWindow
{
    //依赖模式的key
    const string isDependPrefKey = "ReferenceFinderData_IsDepend";
    //是否需要更新信息状态的key
    const string needUpdateStatePrefKey = "ReferenceFinderData_needUpdateState";

    private static ReferenceFinderData data = new ReferenceFinderData();
    private static bool initializedData = false;
    
    private bool isDepend = false;
    private bool needUpdateState = true;

    private bool needUpdateAssetTree = false;
    private bool initializedGUIStyle = false;
    //工具栏按钮样式
    private GUIStyle toolbarButtonGUIStyle;
    //工具栏样式
    private GUIStyle toolbarGUIStyle;
    //选中资源列表
    private HashSet<string> selectedAssetGuid = new HashSet<string>();    

    private AssetTreeView m_AssetTreeView;

    [SerializeField]
    private TreeViewState m_TreeViewState;

    
    //查找资源引用信息
    [MenuItem("Assets/Find References In Project %#&f", false, 25)]
    static void FindRef()
    {
      
        
        
        InitDataIfNeeded();
        OpenWindow();
        ReferenceFinderWindow window = GetWindow<ReferenceFinderWindow>();
        window.UpdateSelectedAssets();
    }
    
    //打开窗口
    [MenuItem("Window/Reference Finder", false, 1000)]
    static void OpenWindow()
    {
        ReferenceFinderWindow window = GetWindow<ReferenceFinderWindow>();
        window.wantsMouseMove = false;
        window.titleContent = new GUIContent("Ref Finder");
        window.Show();
        window.Focus();        
    }

    //初始化数据
    static void InitDataIfNeeded()
    {
        if (!initializedData)
        {
            //初始化数据
            if(!data.ReadFromCache())
            {
                data.CollectDependenciesInfo();
            }
            initializedData = true;
        }
    }

    //初始化GUIStyle
    void InitGUIStyleIfNeeded()
    {
        if (!initializedGUIStyle)
        {
            toolbarButtonGUIStyle = new GUIStyle("ToolbarButton");
            toolbarGUIStyle = new GUIStyle("Toolbar");
            initializedGUIStyle = true;
        }
    }
    
    //更新选中资源列表
    private void UpdateSelectedAssets()
    {
        selectedAssetGuid.Clear();
        foreach(var obj in Selection.objects)
        {
            string path = AssetDatabase.GetAssetPath(obj);
            //如果是文件夹
            if (Directory.Exists(path))
            {
                string[] folder = new string[] { path };
                //将文件夹下所有资源作为选择资源
                string[] guids = AssetDatabase.FindAssets(null, folder);
                foreach(var guid in guids)
                {
                   
                    // 默认认为  文件夹名称没有点 
                    string path_full = AssetDatabase.GUIDToAssetPath(guid);
                    int dian_index = 0,fen_index = 0;
                    for (int str_index = path_full.Length-1; str_index >= 0; str_index--)
                    {
                        if (dian_index == 0 && path_full[str_index] == '.'  )
                        {
                            dian_index = str_index;
                        }
                        if(path_full[str_index] == '/')
                        {
                            fen_index = str_index;
                            if(dian_index != 0)
                            {
                                selectedAssetGuid.Add(guid);
                            } 
                            break;
                        }
                    }
                    // Directory.Exists .net 3.5 大量计算后比较耗时，高版本的.net 没有试过 
                }
            }
            //如果是文件资源
            else
            {
                string guid = AssetDatabase.AssetPathToGUID(path);
                selectedAssetGuid.Add(guid);
            }
        }
        needUpdateAssetTree = true;
    }

    //通过选中资源列表更新TreeView
    private void UpdateAssetTree()
    {
        if (needUpdateAssetTree && selectedAssetGuid.Count != 0)
        {
            var root = SelectedAssetGuidToRootItem(selectedAssetGuid);
            if(m_AssetTreeView == null)
            {
                //初始化TreeView
                if (m_TreeViewState == null)
                    m_TreeViewState = new TreeViewState();
                var headerState = AssetTreeView.CreateDefaultMultiColumnHeaderState(position.width);
                var multiColumnHeader = new MultiColumnHeader(headerState);
                m_AssetTreeView = new AssetTreeView(m_TreeViewState, multiColumnHeader);
            }
            if(root != null)
            {
                m_AssetTreeView.assetRoot = root;
                m_AssetTreeView.CollapseAll();
                m_AssetTreeView.Reload();
                needUpdateAssetTree = false;
            }
            
        }
    }

    private void OnEnable()
    {
        isDepend = PlayerPrefs.GetInt(isDependPrefKey, 0) == 1;
        needUpdateState = PlayerPrefs.GetInt(needUpdateStatePrefKey, 1) == 1;
    }

    private void OnGUI()
    {
        InitGUIStyleIfNeeded();
        DrawOptionBar();
        UpdateAssetTree();
        if (m_AssetTreeView != null)
        {
            //绘制Treeview
            m_AssetTreeView.OnGUI(new Rect(0, toolbarGUIStyle.fixedHeight, position.width, position.height - toolbarGUIStyle.fixedHeight));
        }        
    }
    
    //绘制上条
    public void DrawOptionBar()
    {
        EditorGUILayout.BeginHorizontal(toolbarGUIStyle);
        //刷新数据
        if (GUILayout.Button("Refresh Data", toolbarButtonGUIStyle))
        {
            data.CollectDependenciesInfo();
            needUpdateAssetTree = true;
            EditorGUIUtility.ExitGUI();
        }
        //修改模式
        bool PreIsDepend = isDepend;
        isDepend = GUILayout.Toggle(isDepend, isDepend ? "Model(Depend)" : "Model(Reference)", toolbarButtonGUIStyle,GUILayout.Width(100));
        if(PreIsDepend != isDepend){
            OnModelSelect();
        }
        //是否需要更新状态
        bool PreNeedUpdateState = needUpdateState;
        needUpdateState = GUILayout.Toggle(needUpdateState, "Need Update State", toolbarButtonGUIStyle);
        if (PreNeedUpdateState != needUpdateState)
        {
            PlayerPrefs.SetInt(needUpdateStatePrefKey, needUpdateState ? 1 : 0);
        }
        GUILayout.FlexibleSpace();

        //扩展
        if (GUILayout.Button("Expand", toolbarButtonGUIStyle))
        {
            if (m_AssetTreeView != null) m_AssetTreeView.ExpandAll();
        }
        //折叠
        if (GUILayout.Button("Collapse", toolbarButtonGUIStyle))
        {
            if (m_AssetTreeView != null) m_AssetTreeView.CollapseAll();
        }
        EditorGUILayout.EndHorizontal();
    }
    
    private void OnModelSelect()
    {
        needUpdateAssetTree = true;
        PlayerPrefs.SetInt(isDependPrefKey, isDepend ? 1 : 0);
    }


    //生成root相关
    private HashSet<string> updatedAssetSet = new HashSet<string>();
    //通过选择资源列表生成TreeView的根节点
    private AssetViewItem SelectedAssetGuidToRootItem(HashSet<string> selectedAssetGuid)
    {
        updatedAssetSet.Clear();
        int elementCount = 0;
        var root = new AssetViewItem { id = elementCount, depth = -1, displayName = "Root", data = null };
        int depth = 0;
        var stack = new Stack<Tuple<string, int>>();
        var processed = new HashSet<string>(); // 记录已处理的节点
        var memo = new Dictionary<string, AssetViewItem>(); // 记忆化缓存

        foreach (var childGuid in selectedAssetGuid)
        {
            stack.Push(new Tuple<string, int>(childGuid, depth));
        }

        while (stack.Count > 0)
        {
            var current = stack.Pop();
            if (!processed.Contains(current.Item1))
            {
                processed.Add(current.Item1);
                var child = CreateTree(current.Item1, ref elementCount, current.Item2, stack, processed, memo);
                if (child != null)
                    root.AddChild(child);
            }
        }

        updatedAssetSet.Clear();
        return root;
    }

    private AssetViewItem CreateTree(string guid, ref int elementCount, int _depth, Stack<Tuple<string, int>> stack, HashSet<string> processed, Dictionary<string, AssetViewItem> memo)
    {
        if (memo.ContainsKey(guid))
            return memo[guid];

        if (needUpdateState && !updatedAssetSet.Contains(guid))
        {
            data.UpdateAssetState(guid);
            updatedAssetSet.Add(guid);
        }
        ++elementCount;
        AssetDescription referenceData = null;
        if (data.assetDict.ContainsKey(guid))
        {
            referenceData = data.assetDict[guid];
        }
        else
        {
            return null;
        }

        var root = new AssetViewItem { id = elementCount, displayName = referenceData.name, data = referenceData, depth = _depth };
        memo[guid] = root; // 缓存当前节点
        

        var childGuids = isDepend ? referenceData.dependencies : referenceData.references;
        foreach (var childGuid in childGuids)
        {
            //var child = CreateTree(childGuid, ref elementCount, _depth + 1, stack);
            stack.Push(new Tuple<string, int>(childGuid, _depth + 1));
        }

        return root;
    }
}
