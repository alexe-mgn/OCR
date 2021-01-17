#include "debug.h"
#include <iostream>

namespace debug {
    PointerWidget::PointerWidget(QWidget *parent) : QWidget(parent) {
        this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        this->move(100, 100);
        QSize size = QSize(40, 40);
        this->setBaseSize(size);
//        this->setGeometry(100, 100, 100, 100);
        this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->setAttribute(Qt::WA_TransparentForMouseEvents);
        this->setAttribute(Qt::WA_InputMethodTransparent);
        this->label = new QLabel(this);
        this->label->move(0, 0);
        this->label->setFixedSize(size);
        this->label->setStyleSheet((QString("color: white; "
                                           "border: 2px solid red; "
                                           "border-radius: %1px %2px; "
                                           "background-color: black;"
                                           ).arg(
                                                   QString::number(size.width() / 2),
                                                   QString::number(size.height() / 2))
                                                   ));
    }

    void PointerWidget::move(const QPoint &pos) {
        QWidget::move(pos - QPoint(this->size().width() / 2, this->size().height() / 2));
    }

    std::string getEventTypeName(unsigned int type) {
        static const std::map<unsigned int, std::string> eventTypes{
                {
                        0,     "None"},
                {
                        1,     "Timer"},
                {
                        2,     "MouseButtonPress"},
                {
                        3,     "MouseButtonRelease"},
                {
                        4,     "MouseButtonDblClick"},
                {
                        5,     "MouseMove"},
                {
                        6,     "KeyPress"},
                {
                        7,     "KeyRelease"},
                {
                        8,     "FocusIn"},
                {
                        9,     "FocusOut"},
                {
                        23,    "FocusAboutToChange"},
                {
                        10,    "Enter"},
                {
                        11,    "Leave"},
                {
                        12,    "Paint"},
                {
                        13,    "Move"},
                {
                        14,    "Resize"},
                {
                        15,    "Create"},
                {
                        16,    "Destroy"},
                {
                        17,    "Show"},
                {
                        18,    "Hide"},
                {
                        19,    "Close"},
                {
                        20,    "Quit"},
                {
                        21,    "ParentChange"},
                {
                        131,   "ParentAboutToChange"},
                {
                        22,    "ThreadChange"},
                {
                        24,    "WindowActivate"},
                {
                        25,    "WindowDeactivate"},
                {
                        26,    "ShowToParent"},
                {
                        27,    "HideToParent"},
                {
                        31,    "Wheel"},
                {
                        33,    "WindowTitleChange"},
                {
                        34,    "WindowIconChange"},
                {
                        35,    "ApplicationWindowIconChange"},
                {
                        36,    "ApplicationFontChange"},
                {
                        37,    "ApplicationLayoutDirectionChange"},
                {
                        38,    "ApplicationPaletteChange"},
                {
                        39,    "PaletteChange"},
                {
                        40,    "Clipboard"},
                {
                        42,    "Speech"},
                {
                        43,    "MetaCall"},
                {
                        50,    "SockAct"},
                {
                        132,   "used"},
                {
                        52,    "DeferredDelete"},
                {
                        60,    "DragEnter"},
                {
                        61,    "DragMove"},
                {
                        62,    "DragLeave"},
                {
                        63,    "Drop"},
                {
                        64,    "DragResponse"},
                {
                        68,    "ChildAdded"},
                {
                        69,    "ChildPolished"},
                {
                        71,    "ChildRemoved"},
                {
                        73,    "ShowWindowRequest"},
                {
                        74,    "PolishRequest"},
                {
                        75,    "Polish"},
                {
                        76,    "LayoutRequest"},
                {
                        77,    "UpdateRequest"},
                {
                        78,    "UpdateLater"},
                {
                        79,    "EmbeddingControl"},
                {
                        80,    "ActivateControl"},
                {
                        81,    "DeactivateControl"},
                {
                        82,    "ContextMenu"},
                {
                        83,    "InputMethod"},
                {
                        87,    "TabletMove"},
                {
                        88,    "LocaleChange"},
                {
                        89,    "LanguageChange"},
                {
                        90,    "LayoutDirectionChange"},
                {
                        91,    "Style"},
                {
                        92,    "TabletPress"},
                {
                        93,    "TabletRelease"},
                {
                        94,    "OkRequest"},
                {
                        95,    "HelpRequest"},
                {
                        96,    "IconDrag"},
                {
                        97,    "FontChange"},
                {
                        98,    "EnabledChange"},
                {
                        99,    "ActivationChange"},
                {
                        100,   "StyleChange"},
                {
                        101,   "IconTextChange"},
                {
                        102,   "ModifiedChange"},
                {
                        109,   "MouseTrackingChange"},
                {
                        103,   "WindowBlocked"},
                {
                        104,   "WindowUnblocked"},
                {
                        105,   "WindowStateChange"},
                {
                        106,   "ReadOnlyChange"},
                {
                        110,   "ToolTip"},
                {
                        111,   "WhatsThis"},
                {
                        112,   "StatusTip"},
                {
                        113,   "ActionChanged"},
                {
                        114,   "ActionAdded"},
                {
                        115,   "ActionRemoved"},
                {
                        116,   "FileOpen"},
                {
                        117,   "Shortcut"},
                {
                        51,    "ShortcutOverride"},
                {
                        118,   "WhatsThisClicked"},
                {
                        120,   "ToolBarChange"},
                {
                        121,   "ApplicationActivate"},
                {
                        122,   "ApplicationDeactivate"},
                {
                        123,   "QueryWhatsThis"},
                {
                        124,   "EnterWhatsThisMode"},
                {
                        125,   "LeaveWhatsThisMode"},
                {
                        126,   "ZOrderChange"},
                {
                        127,   "HoverEnter"},
                {
                        128,   "HoverLeave"},
                {
                        129,   "HoverMove"},
                {
                        152,   "AcceptDropsChange"},
                {
                        154,   "ZeroTimerEvent"},
                {
                        155,   "GraphicsSceneMouseMove"},
                {
                        156,   "GraphicsSceneMousePress"},
                {
                        157,   "GraphicsSceneMouseRelease"},
                {
                        158,   "GraphicsSceneMouseDoubleClick"},
                {
                        159,   "GraphicsSceneContextMenu"},
                {
                        160,   "GraphicsSceneHoverEnter"},
                {
                        161,   "GraphicsSceneHoverMove"},
                {
                        162,   "GraphicsSceneHoverLeave"},
                {
                        163,   "GraphicsSceneHelp"},
                {
                        164,   "GraphicsSceneDragEnter"},
                {
                        165,   "GraphicsSceneDragMove"},
                {
                        166,   "GraphicsSceneDragLeave"},
                {
                        167,   "GraphicsSceneDrop"},
                {
                        168,   "GraphicsSceneWheel"},
                {
                        169,   "KeyboardLayoutChange"},
                {
                        170,   "DynamicPropertyChange"},
                {
                        171,   "TabletEnterProximity"},
                {
                        172,   "TabletLeaveProximity"},
                {
                        173,   "NonClientAreaMouseMove"},
                {
                        174,   "NonClientAreaMouseButtonPress"},
                {
                        175,   "NonClientAreaMouseButtonRelease"},
                {
                        176,   "NonClientAreaMouseButtonDblClick"},
                {
                        177,   "MacSizeChange"},
                {
                        178,   "ContentsRectChange"},
                {
                        179,   "MacGLWindowChange"},
                {
                        180,   "FutureCallOut"},
                {
                        181,   "GraphicsSceneResize"},
                {
                        182,   "GraphicsSceneMove"},
                {
                        183,   "CursorChange"},
                {
                        184,   "ToolTipChange"},
                {
                        185,   "NetworkReplyUpdated"},
                {
                        186,   "GrabMouse"},
                {
                        187,   "UngrabMouse"},
                {
                        188,   "GrabKeyboard"},
                {
                        189,   "UngrabKeyboard"},
                {
                        191,   "MacGLClearDrawable"},
                {
                        192,   "StateMachineSignal"},
                {
                        193,   "StateMachineWrapped"},
                {
                        194,   "TouchBegin"},
                {
                        195,   "TouchUpdate"},
                {
                        196,   "TouchEnd"},
                {
                        197,   "NativeGesture"},
                {
                        199,   "RequestSoftwareInputPanel"},
                {
                        200,   "CloseSoftwareInputPanel"},
                {
                        203,   "WinIdChange"},
                {
                        198,   "Gesture"},
                {
                        202,   "GestureOverride"},
                {
                        204,   "ScrollPrepare"},
                {
                        205,   "Scroll"},
                {
                        206,   "Expose"},
                {
                        207,   "InputMethodQuery"},
                {
                        208,   "OrientationChange"},
                {
                        209,   "TouchCancel"},
                {
                        210,   "ThemeChange"},
                {
                        211,   "SockClose"},
                {
                        212,   "PlatformPanel"},
                {
                        213,   "StyleAnimationUpdate"},
                {
                        214,   "ApplicationStateChange"},
                {
                        215,   "WindowChangeInternal"},
                {
                        216,   "ScreenChangeInternal"},
                {
                        217,   "PlatformSurface"},
                {
                        218,   "Pointer"},
                {
                        219,   "TabletTrackingChange"},
                {
                        1000,  "User"},
                {
                        65535, "MaxUser"}};
        return eventTypes.at(type);
    }
}
