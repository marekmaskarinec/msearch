#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "../rawdraw/CNFG.h"

#include "msearch.h"

extern menu m;

extern short padding;
extern short font_size;

char uppercase(char c) { // this is for gb keyboard. TODO
	switch (c) {
	case '1': return '!';
	case '2': return '"';
	case '4': return '$';
	case '5': return '%';
	case '6': return '^';
	case '7': return '&';
	case '8': return '*';
	case '9': return '(';
	case '0': return ')';
	case '-': return '_';
	case '=': return '+';
	case '[': return '{';
	case ']': return '}';
	case '#': return '~';
	case '\'': return ':';
	case '/': return '?';
	case '.': return '>';
	case ',': return '<';
	case '\\': return '|';
	default: return toupper(c);
	}

	return c;
}

int domovkey(char c) {
	switch (c) {
	case '0':
	case '^': return 0;
	case '$': return m.buf_len;
	case 'h':
		if (m.cursor)
			return m.cursor - 1;
	case 'l':
		if (m.cursor < m.buf_len - 1)
			return m.cursor + 1;
	case 'e':
	case 'w':;
		int pos = m.cursor;
		for (; pos < m.buf_len && m.buffer[pos] != ' '; pos++)
			;

		if (c == 'w' && pos < m.buf_len - 1)
			pos++;
		return pos;
	case 'b':;
		pos = m.cursor;
		if (pos)
			pos--;
		for (; pos && m.buffer[pos] != ' '; pos--)
			;

		return pos;
	default: return m.cursor;
	}
	return m.cursor;
}

void HandleKey(int keycode, int bDown) {
	if (keycode == CNFG_KEY_SHIFT)
		m.shift = !m.shift;
	else if (m.insert) {
		switch (keycode) {
		case CNFG_KEY_BACKSPACE:
			if (!bDown || !m.cursor)
				break;
  
			strcpy(&m.buffer[m.cursor-1], &m.buffer[m.cursor]);
			m.cursor--;
			m.buf_len--;
			m.resultc = match(m.buffer, m.results, m.max_results);
  
			break;
		case CNFG_KEY_LEFT_ARROW:
			if (!bDown)
				break;
  
			if (m.cursor)
				m.cursor--;
			break;
		case CNFG_KEY_RIGHT_ARROW:
			if (!bDown)
				break;
  
			if (m.cursor < m.buf_len)
				m.cursor++;
			break;
  
		case CNFG_KEY_TOP_ARROW:
			if (!bDown)
				break;
  
			if (!m.selection)
				m.selection = m.resultc - 1;
			else
				m.selection--;
  
			break;
		case CNFG_KEY_BOTTOM_ARROW:
			if (!bDown)
				break;
  
			m.selection++;
  
			if (m.selection == m.resultc)
				m.selection = 0;
  
			break;
		case CNFG_KEY_ENTER:
			printf("%s", m.results[m.selection]);
			die();
			break;
		case CNFG_KEY_ESCAPE:
			m.insert = false;
		default:
			if (!bDown || m.cursor == BUFFER_SIZE)
				break;
  
			if (keycode > 255) // rules out special keys on linux
				break;
  
			if (m.cursor != m.buf_len) {
				char tmp[BUFFER_SIZE] = {0};
				strcpy(tmp, &m.buffer[m.cursor]);
				strcpy(&m.buffer[m.cursor+1], tmp); // shift all characters
			}
  
			m.buffer[m.cursor] = m.shift ? uppercase(keycode) : keycode;
			m.cursor++;
			m.buf_len++;
  
			m.resultc = match(m.buffer, m.results, m.max_results);
			break;
		}
	} else {
		if (!bDown)
			return;

		switch (keycode) {
		case 'i':
			m.insert = true;
			break;
		case 'x':
			m.buffer[m.cursor] = 0;
			strcpy(&m.buffer[m.cursor], &m.buffer[m.cursor + 1]);
			break;
		case CNFG_KEY_ESCAPE:
			printf("%s", m.results[m.selection]);
			die();
			break;
		default:
			if (m.shift)
				keycode = uppercase(keycode);	
      
			if (strchr("$^webhl", keycode))
				m.cursor = domovkey(keycode);
		}
	}
}

void HandleButton( int x, int y, int button, int bDown ) {
	if (!bDown)
		return;

	if (y < padding + font_size * 6 || y > padding + (font_size * 6) * (m.resultc + 1))
		return;

	int old = m.selection;
	m.selection = (y - padding - font_size * 6) / (font_size * 6);

	if (old == m.selection) {
		printf("%s", m.results[m.selection]);
		die();
	}
}
