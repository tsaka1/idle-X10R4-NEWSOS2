/*
 * idle.c - Display Idle (X10/TTY)
 *
 *	Written by saka@ulis.junet (Sakaguchi, Tetsuo)
 *
 * Program name:	idle
 * Parameters:
 *	-X:		X10 mode (default if DISPLAY exists in env.)
 *	-T:		TTY mode (default if DISPLAY does not exist in env.)
 *	-b:		screen blank mode
 *	-m:		moving icon mode (default)
 *	-F {color}:	foreground color (color X10 only, default: white)
 *	-B {color}:	background color (color X10 only, default: black)
 *	-r:		reverse color (X10 only)
 *	-i {file/word}:	set icon bitmap file (X10) or icon word (TTY)
 *	-D {display}:	display name (X10 only)
 */

char RCSHeader[] = "$Header: idle.c,v 1.14 88/10/11 14:46:19 saka Exp $";

#include <stdio.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <strings.h>
#include <X/Xlib.h>

/* default icons */
#include "csr.icon"
#include "csr_mask.icon"
#include "idle.icon"
#define IDLE_WORD	"NEWS"

/* global variables */
char *name;					/* program name */
char *fg_color = NULL, *bg_color = NULL;	/* color name */
Color foreground, background;			/* colors */
int rv_mode = 0;				/* reverse flag */
char *icon_name = NULL;				/* icon file/word */
int icon_width = idle_width;			/* width of bitmap icon */
int icon_height = idle_height; 			/* height of bitmap icon */
short *icon_bits = idle_bits;			/* bitmap of icon */
char *icon_word;				/* word of icon */
char *env_display = NULL;			/* env. DISPLAY */
Window wd;					/* window for idle */
Cursor csr;					/* cursor (none) */
Display *display;				/* X-display */
Bitmap bmask;					/* bitmap mask for icon */
int width, height;				/* W & H of display */

char *getenv();



/* Initialize X10 idle window */
initX10 ()
{
	Color tmp_color;
	Pixmap window_bg;	/* background pixmap */
	
	/* Load bitmap file, if needed */
	if (icon_name != NULL) {
		if (XReadBitmapFile(icon_name, &icon_width, &icon_height,
				&icon_bits, NULL, NULL) != 1) {
			fprintf(stderr, "%s: Can't read bitmap file\n", name);
			exit(1);
		}
	}
	/* Connecting to X10-server */
	if ((display = XOpenDisplay(env_display)) == 0) {
		fprintf(stderr, "%s: Can't open display\n", name);
		exit(1);
	}
	/* Set foreground & background colors */
	if (DisplayPlanes() > 1) {
		/* on color display */
		if (fg_color != NULL) {
			if (XParseColor(fg_color, &foreground) == 0) {
				fprintf(stderr, "%s: Bad color\n", name);
				exit(1);
			}
		} else {
			XParseColor("#ffffffffffff", &foreground);
		}
		if (bg_color != NULL) {
			if (XParseColor(bg_color, &background) == 0) {
				fprintf(stderr, "%s: Bad color\n", name);
				exit(1);
			}
		} else {
			XParseColor("#000000000000", &background);
		}
		XGetHardwareColor(&foreground);
		XGetHardwareColor(&background);
	} else {
		/* on monochrome display */
		foreground.pixel = WhitePixel;
		background.pixel = BlackPixel;
	}
	if (rv_mode != 0) {
		tmp_color = foreground;
		foreground = background;
		background = tmp_color;
	}
	/* Create idle window */
	window_bg = XMakePixmap(0, background.pixel, background.pixel);
	width = DisplayWidth(display);
	height = DisplayHeight(display);
	if ((wd = XCreateWindow(RootWindow, 0, 0, width, height, 0, 0,
			window_bg)) == 0) {
		fprintf(stderr, "%s: Can't create window\n", name);
		exit(1);
	}
	/* Set null cursor and other property */
	csr = XCreateCursor(csr_width, csr_height, csr_bits, csr_mask_bits,
			csr_x_hot, csr_y_hot, background.pixel,
			background.pixel, GXcopy);
	bmask = XStoreBitmap(icon_width, icon_height, icon_bits);
	XStoreName(wd, name);
	XDefineCursor(wd, csr);
	XMapWindow(wd);
	XClear(wd);
	XSelectInput(wd, KeyPressed | ButtonPressed);
	return (0);
}

/* Re-draw display (X10), when SIGALRM */
redrawX10 ()
{
	int x, y;
	
	/* Hide all other windows */
	XRaiseWindow(wd);
	/* Re-drawing */
	XClear(wd);
	x = rand() % (width - icon_width);
	y = rand() % (height - icon_height);
	XBitmapBitsPut(wd, x, y, icon_width, icon_height, icon_bits,
			foreground.pixel, background.pixel, bmask,
			GXcopy, AllPlanes);
	XFlush();
	return (0);
}

/* Wait for KEY/BUTTON press */
waitingX10 ()
{
	XEvent event;
	
	XNextEvent(&event);
	return (0);
}

/* Idle window destory */
resetX10 ()
{
	XFreeCursor(csr);
	XFreeBitmap(bmask);
	XDestroyWindow(wd);
	return (0);
}



/* Initialize TTY display */
initTTY ()
{
	char bp[1024];
	
	/* Set icon word */
	if (icon_name != NULL) {
		icon_word = icon_name;
	} else {
		icon_word = IDLE_WORD;
	}
	/* Check termcap & initialize curses package */
	if (tgetent(bp, getenv("TERM")) != 1) {
		fprintf(stderr, "%s: Can't open termcap", name);
		exit(1);
	}
	width = tgetnum("co");
	height = tgetnum("li");
	if (width < 0 || height < 0) {
		fprintf(stderr, "%s: No 'co' or 'li' capability\n", name);
		exit(1);
	}
	initscr();
	raw();
	setbuf(stdin, NULL);
	clear();
	refresh();
	return (0);
}

/* Re-draw display (TTY), when SIGALRM */
redrawTTY ()
{
	int x, y;

	/* Re-drawing */	
	clear();
	x = rand() % (width - strlen(icon_word));
	y = rand() % (height - strlen(icon_word));
	move(y, x);
	addstr(icon_word);
	refresh();
	return (0);
}

/* Wait for KEY press */
waitingTTY ()
{
	(void) getchar();
	return (0);
}

/* Reset display (TTY) */
resetTTY ()
{
	endwin();
	return (0);
}



/* for screen blank re-drawing (do nothing) */
redrawBLANK ()
{
	return (0);
}

/* parameter error subroutine */
bad_param ()
{
	fprintf(stderr, "%s: Bad parameter\n", name);
	exit(1);
}

/* Main routine */
main (argc, argv)
int argc;
char **argv;
{
	int (*init_fn)() = NULL;	/* initialize function entry */
	int (*re_draw_fn)() = NULL;	/* re-draw function entry */
	int (*waiting_fn)() = NULL;	/* wait function entry */
	int (*reset_fn)() = NULL;	/* reset function entry */
	struct itimerval it_val, it_dummy;

	/* Setting mode by specified parameters */
	name = *argv++;
	argc--;
	while (argc > 0) {
		if (**argv != '-') bad_param();
		switch (*(*argv + 1)) {
			case 'X':	/* select X10 display mode */
				init_fn = initX10;
				if (re_draw_fn != redrawBLANK) {
					re_draw_fn = redrawX10;
				}
				waiting_fn = waitingX10;
				reset_fn = resetX10;
				break;
			case 'T':	/* select TTY display mode */
				init_fn = initTTY;
				if (re_draw_fn != redrawBLANK) {
					re_draw_fn = redrawTTY;
				}
				waiting_fn = waitingTTY;
				reset_fn = resetTTY;
				break;
			case 'b':	/* select blank idle mode */
				re_draw_fn = redrawBLANK;
				break;
			case 'm':	/* select moving idle mode */
				/* do nothing */
				break;
			case 'F':	/* set foreground color */
				if (--argc < 1) bad_param();
				fg_color = *(++argv);
				break;
			case 'B':	/* set background color */
				if (--argc < 1) bad_param();
				bg_color = *(++argv);
				break;
			case 'r':	/* reverse color mode */
				rv_mode = -1;
				break;
			case 'i':	/* set specified icon */
				if (--argc < 1) bad_param();
				icon_name = *(++argv);
				break;
			case 'D':	/* set DISPLAY */
				if (--argc < 1) bad_param();
				env_display = *(++argv);
				break;
			default:	/* error */
				bad_param();
				break;
		}
		argv++;
		argc--;
	}
	/* Set default mode by environment */
	if (init_fn == NULL) {	/* Not set display mode */
		if (env_display != NULL) {
			/* DISPLAY specified, set X10 mode */
			init_fn = initX10;
			if (re_draw_fn != redrawBLANK) re_draw_fn = redrawX10;
			waiting_fn = waitingX10;
			reset_fn = resetX10;
		} else {
			/* DISPLAY not specified */
			if ((env_display = getenv("DISPLAY")) != NULL) {
				/* env.DISPLAY exists, set X10 mode */
				init_fn = initX10;
				if (re_draw_fn != redrawBLANK) {
					re_draw_fn = redrawX10;
				}
				waiting_fn = waitingX10;
				reset_fn = resetX10;
			} else {
				/* env.DISPLAY not exists, set TTY mode */
				init_fn = initTTY;
				if (re_draw_fn != redrawBLANK) {
					re_draw_fn = redrawTTY;
				}
				waiting_fn = waitingTTY;
				reset_fn = resetTTY;
			}
		}
	}
	/* Initialize display */
	init_fn();
	/* Set SIGALRM vector */
	signal(SIGALRM, re_draw_fn);
	/* Set interval timer */
	it_val.it_interval.tv_sec = 2;
	it_val.it_interval.tv_usec = 0;
	it_val.it_value.tv_sec = 2;
	it_val.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it_val, &it_dummy);
	/* Wait for KEY/(BUTTON) press */
	waiting_fn();
	/* Unset interval timer */
	it_val.it_interval.tv_sec = 0;
	it_val.it_interval.tv_usec = 0;
	it_val.it_value.tv_sec = 0;
	it_val.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it_val, &it_dummy);
	signal(SIGALRM, SIG_DFL);
	/* Reset display */
	reset_fn();
	exit(0);
}
