# Qt 微信桌面端高仿 Demo

## 架构设计

本项目采用 Qt Widgets + MVVM 风格组织代码：

- `models`：定义 `Message`、`Conversation`、`Contact` 等纯数据结构。
- `data`：`DatabaseManager` 负责 SQLite 建表、种子数据、消息/会话/联系人读写。
- `viewmodels`：`ChatViewModel` 聚合业务状态，向 UI 暴露会话、联系人、消息列表，并处理发送、置顶、未读、隐藏、清空等业务意图。
- `widgets`：各类自定义控件，包含无边框标题栏、导航栏、会话列表项、聊天气泡、输入工具栏、个人资料与设置面板。
- `resources`：集中存放 QSS 主题，模拟微信桌面端浅灰背景、绿色气泡、红点角标、Hover/Pressed 状态。

## 目录结构

```text
wxdemo/
├── wxdemo.pro
├── README.md
├── resources/
│   ├── resources.qrc
│   └── styles/wechat.qss
└── src/
    ├── main.cpp
    ├── models/
    │   ├── Message.h
    │   ├── Message.cpp
    │   ├── Conversation.h
    │   └── Contact.h
    ├── data/
    │   ├── DatabaseManager.h
    │   └── DatabaseManager.cpp
    ├── viewmodels/
    │   ├── ChatViewModel.h
    │   └── ChatViewModel.cpp
    └── widgets/
        ├── ChatWindow.*
        ├── TitleBar.*
        ├── NavigationBar.*
        ├── ConversationItemWidget.*
        ├── ChatMessageWidget.*
        ├── ChatInputWidget.*
        ├── AvatarWidget.*
        ├── ProfileDialog.*
        └── SettingsDialog.*
```

## 构建运行

```powershell
qmake wxdemo.pro
mingw32-make -j4
.\release\wxdemo.exe
```

如果使用 MSVC，请先打开对应的 Qt/MSVC 开发者命令行，再执行 `qmake` 与 `nmake`。
