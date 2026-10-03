#ifndef TUILIB_H
#define TUILIB_H

#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef TUILIB_DEF
#	define TUILIB_DEF
#endif // TUILIB_DEF

typedef struct {
	int x;
	int y;
} Tui_Vector2;

typedef struct {
	int code;
} Tui_Color16;

typedef struct {
	char content;
	Tui_Color16 fg;
	Tui_Color16 bg;
} Tui_Cell;


#define TUI_CLEAR_COLOR (Tui_Color16) { 0 }
#define TUI_BLACK_FG   (Tui_Color16) { 30 }
#define TUI_RED_FG     (Tui_Color16) { 31 }
#define TUI_GREEN_FG   (Tui_Color16) { 32 }
#define TUI_YELLOW_FG  (Tui_Color16) { 33 }
#define TUI_BLUE_FG    (Tui_Color16) { 34 }
#define TUI_MAGENTA_FG (Tui_Color16) { 35 }
#define TUI_CYAN_FG    (Tui_Color16) { 36 }
#define TUI_WHITE_FG   (Tui_Color16) { 37 }
#define TUI_BLACK_BG   (Tui_Color16) { 40 }
#define TUI_RED_BG     (Tui_Color16) { 41 }
#define TUI_GREEN_BG   (Tui_Color16) { 42 }
#define TUI_YELLOW_BG  (Tui_Color16) { 43 }
#define TUI_BLUE_BG    (Tui_Color16) { 44 }
#define TUI_MAGENTA_BG (Tui_Color16) { 45 }
#define TUI_CYAN_BG    (Tui_Color16) { 46 }
#define TUI_WHITE_BG   (Tui_Color16) { 47 }

TUILIB_DEF void tui_init_terminal(void);
TUILIB_DEF void tui_close_terminal(void);

TUILIB_DEF bool tui_should_exit(void);
TUILIB_DEF void tui_begin_drawing(void);
TUILIB_DEF void tui_end_drawing(void);
TUILIB_DEF int  tui_get_term_width(void);
TUILIB_DEF int  tui_get_term_height(void);

TUILIB_DEF void tui_clear_background(Tui_Color16 color);
TUILIB_DEF void tui_draw_pixel(char content, int x, int y);
TUILIB_DEF void tui_draw_pixel_v(char content, Tui_Vector2 coords);
TUILIB_DEF void tui_draw_pixel_cv(Tui_Cell cell, Tui_Vector2 coords);


typedef struct {
	struct {
		int width;
		int height;
		Tui_Cell* content;
	} term;

} Tui_Terminal_State;

TUILIB_DEF extern Tui_Terminal_State TUILIB_TERMINAL;

#ifdef TUILIB_IMPLEMENTATION

Tui_Terminal_State TUILIB_TERMINAL = {0};

TUILIB_DEF void tui_init_terminal(void) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	TUILIB_TERMINAL.term.width  = w.ws_col;
	TUILIB_TERMINAL.term.height = w.ws_row;
	TUILIB_TERMINAL.term.content = calloc(
		(size_t)tui_get_term_width() * (size_t)tui_get_term_height(),
		sizeof(Tui_Cell)
	);
}

TUILIB_DEF void tui_close_terminal(void) {
	printf("\033[?1049l");
}

TUILIB_DEF int  tui_get_term_width(void) {
	return TUILIB_TERMINAL.term.width;
}

TUILIB_DEF int  tui_get_term_height(void) {
	return TUILIB_TERMINAL.term.height;
}

TUILIB_DEF bool tui_should_exit(void) {
	return false;
} 

TUILIB_DEF void tui_begin_drawing(void) {
	for (int i = 0; i < tui_get_term_height() * tui_get_term_width(); ++i) {
		TUILIB_TERMINAL.term.content[i].content = ' ';
	}

	printf("\033[?1049h");
	printf("\033[?25l");
}

TUILIB_DEF void tui_end_drawing(void) {
	for (int y = 0; y < tui_get_term_height(); ++y) {
		for (int x = 0; x < tui_get_term_width(); ++x) {
			Tui_Cell cell = TUILIB_TERMINAL.term.content[y * tui_get_term_width() + x];
			printf("\033[%im\033[%im%c\033[0m", cell.fg.code, cell.bg.code, cell.content);
		}
		printf("\n");
	}
}

TUILIB_DEF void tui_draw_pixel(char content, int x, int y) {
	TUILIB_TERMINAL.term.content[y * tui_get_term_width() + x] = (Tui_Cell){
		.content = content,
		.fg = TUI_CLEAR_COLOR,
		.bg = TUI_CLEAR_COLOR,
	};
}

TUILIB_DEF void tui_draw_pixel_v(char content, Tui_Vector2 coords) {
	tui_draw_pixel(content, coords.x, coords.y);
}

TUILIB_DEF void tui_draw_pixel_cv(Tui_Cell cell, Tui_Vector2 coords) {
	TUILIB_TERMINAL.term.content[coords.y * tui_get_term_width() + coords.x] = cell;
}

TUILIB_DEF void tui_clear_background(Tui_Color16 color) {
	for (int i = 0; i < tui_get_term_height() * tui_get_term_width(); ++i) {
		TUILIB_TERMINAL.term.content[i].bg = color;
	}
}

#endif //TUILIB_IMPLEMENTATION

#ifdef TUILIB_NAMESPACE
	#define Color16 Tui_Color16
	
	#define BLACK_FG        TUI_BLACK_FG
	#define RED_FG          TUI_RED_FG    
	#define GREEN_FG        TUI_GREEN_FG  
	#define YELLOW_FG       TUI_YELLOW_FG 
	#define BLUE_FG         TUI_BLUE_FG   
	#define MAGENTA_FG      TUI_MAGENTA_FG
	#define CYAN_FG         TUI_CYAN_FG   
	#define WHITE_FG        TUI_WHITE_FG  
	#define BLACK_BG        TUI_BLACK_BG  
	#define RED_BG          TUI_RED_BG    
	#define GREEN_BG        TUI_GREEN_BG  
	#define YELLOW_BG       TUI_YELLOW_BG 
	#define BLUE_BG         TUI_BLUE_BG   
	#define MAGENTA_BG      TUI_MAGENTA_BG
	#define CYAN_BG         TUI_CYAN_BG   
	#define WHITE_BG        TUI_WHITE_BG  
	
	#define init_terminal    tui_init_terminal
	#define close_terminal   tui_close_terminal
	
	#define should_exit      tui_should_exit
	#define begin_drawing    tui_begin_drawing
	#define end_drawing      tui_end_drawing
	
	#define clear_background tui_clear_background
#endif // TUI_NAMESPACE
#endif // TUILIB_H
