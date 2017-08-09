/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx   = 3;   /* border pixel of windows */
static const unsigned int snap       = 14;  /* snap pixel */
static const int showbar             = 1;   /* 0 means no bar */
static const int topbar              = 1;   /* 0 means bottom bar */
static const char *fonts[]           = { "droid sans:size=14" };
static const char col_bg[]           = "#0C131A";
static const char col_fg[]           = "#E0E0E0";
static const char col_bdsel[]        = "#613A57";
static const char col_fgsel[]        = "#FFFFFF";
static const char col_bgsel[]        = "#613A57";
const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg,    col_bg,    col_bg },
	[SchemeSel]  = { col_fgsel, col_bgsel, col_bdsel },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "feh",          NULL,       NULL,       0,            True,        -1 },
	{ "mpv",          NULL,       NULL,       0,            True,        -1 },
	{ "Transmission", NULL,       NULL,       1 << 6,       False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[ T ]",	tile },		/* first entry is default */
	{ "[ F ]",	NULL },		/* no layout function means floating behavior */
	{ "[ M ]",	monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
/* Defines WINKEY to equal to Super */
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
char dmenumon[2]         = "0"; /* component of dmenucmd, manipulated in spawn() */
const char *dmenucmd[]     = { "dmenu_run", NULL };
const char *st[]           = { "gnome-terminal", NULL };
const char *urxvt[]        = { "urxvt", NULL };
const char *mute[]         = { "amixer", "-q", "set", "Master", "toggle", "nocap", NULL };
const char *volu[]         = { "amixer", "-q", "sset", "Master", "2%+", NULL };
const char *vold[]         = { "amixer", "-q", "sset", "Master", "2%-", NULL };
const char *dpms[]         = { "dash", "-c", "sleep 0.30 && xset dpms force off", NULL };
const char *lcdu[]         = { "xbacklight", "-inc", "5", NULL };
const char *lcdd[]         = { "xbacklight", "-dec", "5", NULL };
const char *prntscrcmd[]   = { "scrot", "-q", "100", NULL };
const char *slock[]        = { "dash", "-c", "sleep 0.5 && xset dpms force off && slock", NULL };
const char *file_manager[] = { "dash", "-c", "GTK_THEME=Adwaita:dark spacefm", NULL };
const char *mict[]         = { "amixer", "set", "Capture", "toggle", NULL };
const char *cal[]          = { "urxvt", "-e", "python", NULL };
const char *alsa[]         = { "st", "-g", "10x10x10x10", "-e", "alsamixer", NULL };
const char *firefox[]      = { "firefox", NULL };
const char *chromium[]     = { "chromium", NULL };


// XK_apostrophe XK_semicolon XK_colon XK_slash
/* Custom Commands */
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = volu } },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = vold } },
	{ 0,                     XF86XK_AudioMute, spawn,          {.v = mute } },
	{ 0,               XF86XK_MonBrightnessUp, spawn,          {.v = lcdu } },
	{ 0,             XF86XK_MonBrightnessDown, spawn,          {.v = lcdd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = st } },
	{ MODKEY|ShiftMask,         XK_apostrophe, spawn,          {.v = urxvt } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = dpms } },
	{ 0,                        XF86XK_Tools,  spawn,          {.v = dpms } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = prntscrcmd } },
	{ WINKEY,                       XK_l,      spawn,          {.v = slock } },
	{ MODKEY,                       XK_a,      spawn,          {.v = alsa } },
	{ 0,                      XF86XK_HomePage, spawn,          {.v = chromium } },
	{ MODKEY,                 XF86XK_HomePage, spawn,          {.v = firefox } },
	{ 0,                    XF86XK_MyComputer, spawn,          {.v = file_manager } },
	{ 0,                            XK_F4,     spawn,          {.v = mict } },
	{ 0,                    XF86XK_Calculator, spawn,          {.v = cal } },

/* dwm commands */
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY,                  XK_bracketleft, incnmaster,     {.i = +1 } },
	{ MODKEY,                 XK_bracketright, incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_period, setmfact,       {.f = +0.02} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                    XK_backslash, view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_h,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_l,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkWinTitle,          0,              Button1,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = st } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
/*
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
*/
};


/* Undefined buttons */
/*
	{ 0,                     XF86XK_AudioPlay, spawn,          {.v = play } },
	{ 0,                     XF86XK_AudioStop, spawn,          {.v = stop } },
	{ 0,                     XF86XK_AudioNext, spawn,          {.v = next } },
	{ 0,                     XF86XK_AudioPrev, spawn,          {.v = prev } },
	{ 0,                     XF86XK_Display,   spawn,          {.v = F07 } },
	{ 0,                     XF86XK_WLAN,      spawn,          {.v = F08 } },
	{ 0,                     XF86XK_Tools,     spawn,          {.v = F09 } },
	{ 0,                     XF86XK_Search,    spawn,          {.v = F10 } },
	{ 0,                     XF86XK_LaunchA,   spawn,          {.v = F11 } },
	{ 0,                     XF86XK_Explorer,  spawn,          {.v = F12 } },
*/

/* Unused commands */
/*
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
char *play[] = { "mocp", "-G", NULL };
char *stop[] = { "mocp", "-x", NULL };
char *next[] = { "mocp", "-f", NULL };
char *prev[] = { "mocp", "-r", NULL };
*/
