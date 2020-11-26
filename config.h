/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int gaps = 32;
static const unsigned int gappih    = gaps;       /* horiz inner gap between windows */
static const unsigned int gappiv    = gaps;       /* vert inner gap between windows */
static const unsigned int gappoh    = gaps;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = gaps;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int borderpx  = 8;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Red October:size=28" };
static const char dmenufont[]       = "monospace:size=28";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]        = "#771500";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_red,  col_red  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci-ru.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },/* first entry is default */
	{ "|M|",      centeredmaster },
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	/*{ ">M>",      centeredfloatingmaster },*/
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
/* commands */
static const char myterm[] = "kitty";

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { myterm, NULL };

static const char *kupfer[]  = { "kupfer", NULL };
static const char *rofiwin[]  = { "rofi","-show","window", NULL };
static const char *openfile[]  = { "/home/artour/mysh/openfile", NULL };
static const char *openfilea[]  = { "/home/artour/mysh/openfile-a", NULL };
static const char *rf[]  = { "/home/artour/myshp/rf","-e", NULL };
static const char *rfd[]  = { "/home/artour/myshp/rf","-e","-d", NULL };

static const char *killsh[]  = { "/home/artour/mysh/kill.sh", NULL };

static const char *browser[]  = { "firefox", NULL };
static const char *feed[]  = { myterm,"newsboat", NULL };
static const char *htop[]  = { myterm,"htop", NULL };
static const char *pamacgui[]  = { "pamac-manager", NULL };
static const char *email[]  = { "thunderbird", NULL };

static const char *eng[]  = { "setxkbmap","us", NULL };

static const char *fullscreen[]  = { "/home/artour/mysh/fullscreen.sh", NULL };
/*static const char *restartdwm[]  = { "/home/artour/dwmsh/restartdwm.sh", NULL };*/
static const char *exitcmd[]  = { "/home/artour/dwmsh/exitdwm.sh", NULL };

static const char *dwmbind[]  = { "/home/artour/dwmsh/dwmbind.sh", NULL };
/*static const char *i3bindcmd[]  = { "/home/artour/mysh/i3bind.sh", NULL };*/

#define PrintScreenDWM	    0x0000ff61
//static const char *screenshot[]  = { "flameshot","screen","-p","/home/artour/Pictures/screenshot", NULL };
static const char *screenshot[]  = { "/home/artour/mysh/screenshot.sh", NULL };
static const char *screenshotadvanced[]  = { "gnome-screenshot","-i", NULL };


static const char *volup1[] = { "/home/artour/dwmsh/dwmvol.sh","+1",NULL };
static const char *volup10[] = { "/home/artour/dwmsh/dwmvol.sh","+10", NULL };
static const char *voldown1[] = { "/home/artour/dwmsh/dwmvol.sh","-1",NULL };
static const char *voldown10[] = { "/home/artour/dwmsh/dwmvol.sh","-10",NULL };
static const char *volmute[] = { "/home/artour/dwmsh/dwmvol.sh","m",NULL };

/*mouse*/
static const char *mouseUp[] = { "xdotool","mousemove_relative","--","0","-10",NULL };
static const char *mouseUpFast[] = { "xdotool","mousemove_relative","--","0","-100",NULL };
static const char *mouseUpFine[] = { "xdotool","mousemove_relative","--","0","-1",NULL };

static const char *mouseDown[] = { "xdotool","mousemove_relative","--","0","10",NULL };
static const char *mouseDownFast[] = { "xdotool","mousemove_relative","--","0","100",NULL };
static const char *mouseDownFine[] = { "xdotool","mousemove_relative","--","0","1",NULL };

static const char *mouseLeft[] = { "xdotool","mousemove_relative","--","-10","0",NULL };
static const char *mouseLeftFast[] = { "xdotool","mousemove_relative","--","-100","0",NULL };
static const char *mouseLeftFine[] = { "xdotool","mousemove_relative","--","-1","0",NULL };

static const char *mouseRight[] = { "xdotool","mousemove_relative","--","10","0",NULL };
static const char *mouseRightFast[] = { "xdotool","mousemove_relative","--","100","0",NULL };
static const char *mouseRightFine[] = { "xdotool","mousemove_relative","--","1","0",NULL };

static const char *mouseLeftClick[] = { "xdotool","click","1",NULL };
static const char *mouseMiddleClick[] = { "xdotool","click","2",NULL };
static const char *mouseRightClick[] = { "xdotool","click","3",NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_z,      spawn,          {.v = kupfer } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = rofiwin } },
	{ MODKEY,                       XK_x,      spawn,          {.v = openfile } },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = openfilea } },
	{ MODKEY,                       XK_c,      spawn,          {.v = rf } },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          {.v = rfd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = htop } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = pamacgui } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          {.v = dwmbind } },
	{ MODKEY,                       XK_n,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          {.v = feed } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = email } },
	{ MODKEY|Mod1Mask,              XK_space,  spawn,          {.v = eng } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_d,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_a,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_d,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_Right,  pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_j,      pushup,         {0} },
	{ MODKEY|ShiftMask,             XK_a,      pushup,         {0} },
	{ MODKEY|ShiftMask,             XK_Left,   pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_s,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Down,   setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_w,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Up,     setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_v,      setmfact,       {.f = 1.50} },
	{ MODKEY|ShiftMask,             XK_v,      setmfact,       {.f = 1.35} },
	{ MODKEY,                       XK_g,      togglegaps,     {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask,           XK_Return, focusmaster,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_q,      spawn,          {.v = killsh } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ControlMask,           XK_f,      spawn,          {.v = fullscreen} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = exitcmd } },
	{ 0,          XF86XK_AudioLowerVolume,     spawn,          {.v = voldown10 } },
	{ ShiftMask,  XF86XK_AudioLowerVolume,     spawn,          {.v = voldown1 } },
	{ 0,          XF86XK_AudioRaiseVolume,     spawn,          {.v = volup10 } },
	{ ShiftMask,  XF86XK_AudioRaiseVolume,     spawn,          {.v = volup1 } },
	{ 0,                 XF86XK_AudioMute,     spawn,          {.v = volmute } },
	{ 0,                   PrintScreenDWM,     spawn,          {.v = screenshot } },
	{ MODKEY,              PrintScreenDWM,     spawn,          {.v = screenshotadvanced } },
	{ MODKEY,                       XK_KP_Up,  spawn,          {.v = mouseUp} },
	{ MODKEY|ControlMask,           XK_KP_Up,  spawn,          {.v = mouseUpFast} },
	{ MODKEY|ShiftMask,             XK_KP_Up,  spawn,          {.v = mouseUpFine} },
	{ MODKEY,                      XK_KP_Begin,spawn,          {.v = mouseDown} },
	{ MODKEY|ControlMask,          XK_KP_Begin,spawn,          {.v = mouseDownFast} },
	{ MODKEY|ShiftMask,            XK_KP_Begin,spawn,          {.v = mouseDownFine} },
	{ MODKEY,                       XK_KP_Left,spawn,          {.v = mouseLeft} },
	{ MODKEY|ControlMask,           XK_KP_Left,spawn,          {.v = mouseLeftFast} },
	{ MODKEY|ShiftMask,             XK_KP_Left,spawn,          {.v = mouseLeftFine} },
	{ MODKEY,                      XK_KP_Right,spawn,          {.v = mouseRight} },
	{ MODKEY|ControlMask,          XK_KP_Right,spawn,          {.v = mouseRightFast} },
	{ MODKEY|ShiftMask,            XK_KP_Right,spawn,          {.v = mouseRightFine} },
	{ MODKEY,                       XK_KP_End, spawn,          {.v = mouseLeftClick} },
	{ MODKEY,                       XK_KP_Next,spawn,          {.v = mouseRightClick} },
	{ MODKEY,                       XK_KP_Down,spawn,          {.v = mouseMiddleClick} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};
/*
{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = restartdwm } },
*/


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY|ShiftMask,Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
/*
{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
*/
