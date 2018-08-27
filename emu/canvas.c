#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iup.h>
#include <cd/cd.h>
#include <cd/cdiup.h>
#include "picat.h"

#define PICAT_ERROR -1

void setFont(int);
void setAlign(int);
void setFontSize(int);
void setFontStyle(int);
int getFontValue(char*);
int getFontStyleValue(char*);

static cdCanvas *cdcanvas = NULL;

typedef enum  {Circle, Line, Rect, Triangle, Polygon, Text}TagType;

struct Lines{
  int x1;
  int x2;
  int y1;
  int y2;
};

struct Rects{
  int x1;
  int x2;
  int y1;
  int y2;
  int fill;
};

struct Triangles{
  int x1;
  int x2;
  int x3;
  int y1;
  int y2;
  int y3;
  int fill;
};

struct Circles{
  int x1;
  int x2;
  int y1;
  int y2;
  long angle1;
  long angle2;
  int fill;
};

struct Vertex{
  int x;
  int y;
  struct Vertex* next;
};

struct Polygons{
  int fill;
  struct Vertex* vertices; 
};

struct Texts{
  int x1;
  int x2;
  int y1;
  int y2;
  char* text;
  int align;
  int boarder;
  int font;
  int fontSize;
  int fontStyle;
  int padding;
};

struct Drawing {
  TagType type;
  void* value;
  int color;
  struct Drawing* next;
};

static struct Drawing* drawings = NULL;
static struct Drawing* last = NULL;
static char canvasSize[100];


int c_textSize(){
  TERM text = 	picat_get_call_arg(1,6);
  TERM font = 	picat_get_call_arg(2,6);
  TERM size = 	picat_get_call_arg(3,6);
  TERM style = 	picat_get_call_arg(4,6);
  TERM pWidth = picat_get_call_arg(5,6);
  TERM pHeight =picat_get_call_arg(6,6);
  if(!picat_is_integer(size)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(text) || !picat_is_atom(font) || !picat_is_atom(style)){
    return PICAT_ERROR;
  }
  if(!picat_is_var(pHeight) || !picat_is_var(pWidth)){
    return PICAT_ERROR;
  }
  char* cText = picat_get_atom_name(text);
  char* cFont = picat_get_atom_name(font);
  char* cStyle = picat_get_atom_name(style);

  long cSize = picat_get_integer(size);

  Ihandle *dlg;
  IupOpen(NULL, NULL);
  Ihandle *can = IupCanvas(NULL);
  dlg = IupDialog(IupVbox(can, NULL));
  IupMap(dlg);
  cdcanvas = cdCreateCanvas(CD_IUP, can);
  if(cSize > 0){
    setFontSize((int) cSize);
  }
  int iFont = getFontValue(cFont);
  setFont(iFont);

  int iStyle = getFontStyleValue(cStyle);
  setFontStyle(iStyle);

  int *width = malloc(sizeof(int));
  int *height = malloc(sizeof(int));
  cdCanvasGetTextSize(cdcanvas, cText, width, height);
  picat_unify(pWidth, picat_build_integer((long) *width));
  picat_unify(pHeight, picat_build_integer((long) *height));
  free(width);
  free(height);
  //IupClose();
  return PICAT_TRUE;
}



void setColor(int color){
  switch(color){
    case 1: //red color
      cdCanvasForeground(cdcanvas, CD_RED);
      break;
    case 2: //dark red color
      cdCanvasForeground(cdcanvas, CD_DARK_RED);
      break;
    case 3: //yellow color
      cdCanvasForeground(cdcanvas, CD_YELLOW);
      break;
    case 4: //green color
      cdCanvasForeground(cdcanvas, cdEncodeColor(0,128,0));
      break;
    case 5: //blue color
      cdCanvasForeground(cdcanvas, CD_BLUE);
      break;
    case 6: //pink color
      cdCanvasForeground(cdcanvas, CD_MAGENTA);
      break;
    case 7: //silver color
      cdCanvasForeground(cdcanvas, CD_GRAY);
      break;
    case 8: //black color
      cdCanvasForeground(cdcanvas, CD_BLACK);
      break;
    case 9: //white color
      cdCanvasForeground(cdcanvas, CD_WHITE);
      break;
    case 10: //purple color
      cdCanvasForeground(cdcanvas, cdEncodeColor(128,0,128));
      break;
    case 11: //orange color
      cdCanvasForeground(cdcanvas, cdEncodeColor(255,87,51));
      break;
    case 12: //grey color
      cdCanvasForeground(cdcanvas, cdEncodeColor(128,128,128));
      break;
    case 13: //cyan color
      cdCanvasForeground(cdcanvas, cdEncodeColor(0,255,255));
      break;
    case 14: //gold color
      cdCanvasForeground(cdcanvas, cdEncodeColor(218,165,32));
      break;
    case 15: //lime green color
      cdCanvasForeground(cdcanvas, cdEncodeColor(0,255,0));
      break;
    case 16: //salmon color
      cdCanvasForeground(cdcanvas, cdEncodeColor(250,128,144));
      break;
  }
}


int insertE(void* nextNode, TagType type, int color){
  //create new node and assign data
  struct Drawing* draw = (struct Drawing*) malloc(sizeof(struct Drawing));
  if( draw == NULL){
    printf("Error: Out of Memory");
    return -1;
  }
  draw->type = type;
  draw->value = nextNode;
  draw->color = color;
  draw->next = NULL;
  if(drawings == NULL){
    //empty list
    drawings = draw;
    last = draw;
  } else {
    if(last != NULL){ 
      last->next = draw;
      last = draw;
    }
  }
  return 0;
}

 
void clearList(){
  struct Drawing* node = drawings; 
  struct Drawing* next;
  struct Vertex* cur;
  struct Vertex* nextVer;
  struct Polygons* poly;
  while(node != NULL){
    next = node->next;
    if(node->type == 4){
      poly = node->value;
      cur = poly->vertices;
      while(cur!= NULL){
        nextVer = cur->next;
	free(cur);
        cur = nextVer;
      }
    }
    free(node->value);
    free(node);
    node = next;
  }
  drawings = NULL;
  last = NULL;
}

int redraw_cb( Ihandle *self, float x, float y )
{
  int curColor;
  int curAlign;
  int curFont;
  int curFontSize;
  int curStyle;
  cdCanvasActivate(cdcanvas);
  cdCanvasClear(cdcanvas);
  cdCanvasForeground(cdcanvas, CD_RED);
  
  int *height = malloc(sizeof(int));
  cdCanvasGetSize(cdcanvas, NULL, height, NULL, NULL);
  struct Drawing* node = drawings;
  curColor = 1;
  setAlign(2);
  curAlign = 2;
  setFont(3);
  curFont = 3;
  setFontSize(12);
  curFontSize = 12;
  setFontStyle(3);
  curStyle = 3;
  while(node != NULL){
    if(node->color != curColor){
      setColor(node->color);
      curColor = node->color;
    } 
    switch(node->type){
      case 1: ;
        struct Lines *line = node->value;
        cdCanvasLine(cdcanvas,line->x1,*height-line->y1-1,line->x2,*height-line->y2-1);
        break;
      case 0: ;
        struct Circles *circle = node->value;
        if(circle->fill == 0){
          cdCanvasArc(cdcanvas,circle->x1,*height-circle->y1-1,circle->x2,circle->y2-1,circle->angle1,circle->angle2+circle->angle1);
        }else
          cdCanvasSector(cdcanvas,circle->x1,*height-circle->y1-1,circle->x2,circle->y2-1,circle->angle1,circle->angle2+circle->angle1);
        break;
      case 2: ;
        struct Rects *rect = node->value;
        if(rect->fill == 0){
          cdCanvasRect(cdcanvas,rect->x1,rect->x2,*height-rect->y1-1,*height-rect->y2-1);
        } else {
          cdCanvasBox(cdcanvas,rect->x1,rect->x2,*height-rect->y1-1,*height-rect->y2-1);
        }
        break;
      case 3: ;
        struct Triangles *tri = node->value;
        if(tri->fill == 0){
          cdCanvasBegin(cdcanvas, CD_CLOSED_LINES);
        } else {
          cdCanvasBegin(cdcanvas, CD_FILL);
        }
        cdCanvasVertex(cdcanvas,tri->x1,*height-tri->y1-1);
        cdCanvasVertex(cdcanvas,tri->x2,*height-tri->y2-1);
        cdCanvasVertex(cdcanvas,tri->x3,*height-tri->y3-1);
        cdCanvasEnd(cdcanvas);
        break;
      case 4: ;
	struct Polygons* poly = node->value;
        struct Vertex* vertex = poly->vertices;
        if(poly->fill == 0){
          cdCanvasBegin(cdcanvas, CD_CLOSED_LINES);
	} else {
          cdCanvasBegin(cdcanvas, CD_FILL);
        }
        while(vertex != NULL){
	  cdCanvasVertex(cdcanvas,vertex->x,*height-vertex->y-1);
          vertex = vertex->next;
        }
        cdCanvasEnd(cdcanvas);
	break;
      case 5: ;
        struct Texts* text = node->value;
        if(curAlign != text->align){
	  setAlign(text->align);
          curAlign = text->align;
        }
        if(curFont != text->font){
	  setFont(text->font);
	  curFont = text->font;
        }
	if(curFontSize != text->fontSize){
	  setFontSize(text->fontSize);
	  curFontSize = text->fontSize;
	}
        if(curStyle != text->fontStyle){
	  setFontStyle(text->fontStyle);
	  curStyle = text->fontStyle; 	
	}
        if(text->boarder == 0){
          cdCanvasText(cdcanvas,text->x1,*height-text->y1-1,text->text);
        }
        else if(text->boarder == 1){
          int* w = malloc(sizeof(int));
          int* h = malloc(sizeof(int));
          int pad = text->padding;
          cdCanvasGetTextSize(cdcanvas,text->text,w,h);
	  //The location of the text and rectangle changes with different alignments
          switch(text->align){
            case 1:
              cdCanvasText(cdcanvas,text->x1,*height-text->y1-pad-1,text->text);
	      cdCanvasRect(cdcanvas,text->x1-*w/2-pad,text->x1+*w/2+pad,*height-text->y1-1,*height-text->y1-*h-2*pad-1);
	      break;
            case 2:
              cdCanvasText(cdcanvas,text->x1+pad,*height-text->y1-pad-1,text->text);
              cdCanvasRect(cdcanvas,text->x1,text->x1+*w+2*pad,*height-text->y1-1,*height-text->y1-*h-2*pad-1);
              break;
            case 3:
              cdCanvasText(cdcanvas,text->x1-pad,*height-text->y1-pad-1,text->text);
	      cdCanvasRect(cdcanvas,text->x1-*w-2*pad,text->x1,*height-text->y1-1,*height-text->y1-*h-2*pad-1);
	      break;
          }
          free(w);
          free(h);
        }
        break;
    }
    node = node->next;  
  }

  free(height);
  //clearList();
  return IUP_DEFAULT;
}

int c_clearCanvas(){
  clearList();
  return PICAT_TRUE;
}


int getFill(int fill){
  if( fill > 0)
    return 1;
  else
    return 0;
}

int getFontValue(char* font){
  if((font != NULL) && (font[0] != '\0')){
    switch(font[0]){
      case 'c':
        if(strcmp(font, "courier") == 0){
          return 1;
        }
        break;
      case 'h':
        if(strcmp(font, "helvetica") == 0){
          return 2;
        }
        break;
      case 's':
        if(strcmp(font, "system") == 0){
          return 2;
        }
        break;
      case 't':
        if(strcmp(font, "times") == 0){
          return 4;
        }
        break;
      default:
        return -1;
    }
  }
}

int getFontStyleValue(char* style){
  if((style != NULL) && (style[0] != '\0')){
    switch(style[0]){
      case 'b':
	if(strcmp(style, "bold") == 0){
	  return 1;
	}
      case 'i':
	if(strcmp(style, "italic") == 0){
	  return 2;
	}
	break;
      case 'p':
	if(strcmp(style, "plain") == 0){
	  return 3;
	}
	break;
      default:
	return -1;
    }
  }
  return -1;
}


int getColorValue(char* color){
  if((color != NULL) && (color[0] !='\0')){
    switch(color[0]){
      case 'b':
        if(strcmp(color, "blue") == 0){
          return 5;
        }     
        if(strcmp(color, "black") == 0){
          return 8;
        }
        break;
      case 'c':
        if(strcmp(color, "cyan") == 0){
          return 13;
        }     
        break;
      case 'd':
        if(strcmp(color, "dark_red") == 0 || strcmp(color,"dark red") == 0){
          return 2;
        }
        break;
      case 'g':
        if(strcmp(color, "green") == 0){
          return 4;
        }
        if(strcmp(color, "grey") == 0|| strcmp(color, "gray") == 0){
          return 12;
        }
 	if(strcmp(color, "gold") ==0){
	  return 14;
	}
        break;
      case 'l':
 	if(strcmp(color, "lime") ==0){
	  return 15;
	}
        break;
      case 'o':
	if(strcmp(color, "orange") == 0){
	  return 11;
	}
	break;
      case 'p':
        if(strcmp(color, "pink") == 0){
          return 6;
        } else if ( strcmp(color, "purple") ==0 ){
	  return 10;
	}
        break;
      case 'r':
        if( strcmp(color, "red") == 0){
          return 1;
        }
        break;
      case 's':
	if(strcmp(color, "silver") == 0){
	  return 7;
	}
	if(strcmp(color, "salmon") == 0){
	  return 16;
	}
	break;
      case 'w':
        if(strcmp(color, "white") == 0){
          return 9;
        }
        break;
      case 'y':
        if(strcmp(color, "yellow") == 0){
          return 3;
        }
        break;
      default:
        return -1;
    }
  }
}

int getAlignValue(char* align){
  if((align != NULL) && (align[0] !='\0')){
    switch(align[0]){
      case 'c':
        if(strcmp(align, "center") == 0){
          return 1;
        }
        break;
      case 'l':
        if(strcmp(align, "left") == 0){
          return 2;
        }
        break;
      case 'r':
        if(strcmp(align, "right") == 0){
          return 3;
        }
        break;
      default:
        break;
    }
    return -1;
  }
}

void setFont(int font){
  switch(font){
    case 1:
      cdCanvasFont(cdcanvas,"Courier", -1, 0);
      break;
    case 2:
      cdCanvasFont(cdcanvas,"Helvetica", -1, 0);
      break;
    case 3: 
      cdCanvasFont(cdcanvas,"System", -1, 0);
      break;
    case 4:
      cdCanvasFont(cdcanvas,"Times", -1, 0);
      break;
    default:
      return;
  }
}

void setFontSize(int fontSize){
  cdCanvasFont(cdcanvas, NULL, -1, fontSize);
}



void setFontStyle(int fontStyle){
  switch(fontStyle){
    case 1:
      cdCanvasFont(cdcanvas,NULL, CD_BOLD, 0);
      break;
    case 2:
      cdCanvasFont(cdcanvas,NULL, CD_ITALIC, 0);
      break;
    case 3:
      cdCanvasFont(cdcanvas,NULL, CD_PLAIN, 0);
      break;
  }

}

void setAlign(int align){
  switch(align){
    case 1:
      cdCanvasTextAlignment(cdcanvas, CD_NORTH);
      break;
    case 2:
      cdCanvasTextAlignment(cdcanvas, CD_NORTH_WEST);
      break;
    case 3:
      cdCanvasTextAlignment(cdcanvas, CD_NORTH_EAST);
      break;
    default:
      break;
  }
}
int assignRect(int x1, int x2, int y1, int y2, int color, int fill, TagType type){
  if(type == 2){
    //Rectangle
    struct Rects *rect;
    rect = (struct Rects*) malloc(sizeof(struct Rects));
    if(rect == NULL){
      printf("Error: Out of Memory\n");
      return -1;
    }
    rect->x1 = x1;
    rect->x2 = x2;
    rect->y1 = y1;
    rect->y2 = y2;
    rect->fill = getFill(fill);
    if(insertE(rect, type, color) == 0){
      return 0;
    } else {
      return -1;
    }
  }
  return -1;
}

int assignLine(int x1, int x2, int y1, int y2, TagType type, int color){
  if(type == 1){
    struct Lines *line;
    line = (struct Lines*) malloc(sizeof(struct Lines));
    if(line == NULL){
      printf("Error: Out of Memory\n");
      return -1;
    }
    line->x1 = x1;
    line->x2 = x2;
    line->y1 = y1;
    line->y2 = y2;
    if(insertE(line, type, color) == 0){
      return 0;
    } else {
      return -1;
    }
  } 
  return -1;
}

int assignArc(int x1, int x2, int y1, int y2, long angle1, long angle2, int color, int fill, TagType type){
  if( type == 0){
    struct Circles *circle;
    circle = (struct Circles*) malloc(sizeof(struct Circles));
    if(circle == NULL){
      printf("Error: Out of Memory\n");
      return -1;
    }
    circle->x1 = x1;
    circle->x2 = x2;
    circle->y1 = y1;
    circle->y2 = y2;
    circle->angle1 = angle1;
    circle->angle2 = angle2;
    circle->fill = getFill(fill);
    if(insertE(circle, type, color) == 0){
      return 0;
    } else {
      return -1;
    }
  }
  return -1;
}

int assignStar(int x, int y,int w,int h,int n, int centerX,int centerY, int angle,int diameter,int color, int fill){
  struct Polygons* polygon;  
  struct Vertex* vertex;
  struct Vertex* next;
  polygon = (struct Polygons*) malloc(sizeof(struct Polygons));
  polygon->fill = getFill(fill);
  int arc = 360 / n;
  int alpha = 90 - arc + angle;
  if(alpha<0) alpha += 360;
  int angle1 = alpha + arc / 2;
  int angle2 = alpha + arc;
  for(int i = 0; i < n; i++){
    angle1 = (angle1 >=360 ? angle1-360 : angle1);
    angle2 = (angle2 >=360 ? angle2-360 : angle2);
    double rad1 = ((double)angle1/180.0)*M_PI;
    double rad2 = ((double)angle2/180.0)*M_PI;
    int sx = (int)(centerX-(diameter/2)*cos(rad1));
    int sy = (int)(centerY-(diameter/2)*sin(rad1));
    int lx = (int)(centerX-(h/2)*cos(rad2));
    int ly = (int)(centerY-(h/2)*sin(rad2));
    vertex = (struct Vertex*) malloc(sizeof(struct Vertex));
    if( i == 0){
      polygon->vertices = vertex;
    } else {
      next->next = vertex;
    }
    next = (struct Vertex*) malloc(sizeof(struct Vertex));
    if(vertex == NULL|| next == NULL){
      printf("Error: Out of Memory\n");
      return -1;
    }
    vertex->x = sx;
    vertex->y = sy;
    vertex->next = next;
    next->x = lx;
    next->y = ly;
    angle1 += arc;
    angle2 += arc;
    next->next = NULL;
  }
  if(insertE(polygon, Polygon, color) == 0){
    return 0;
  } else {
    return -1;
  }
}

int assignTri(int x1, int y1, int x2, int y2, int x3, int y3, int fill, TagType type, int color){
  if(type == 3){
    struct Triangles* tri;
    tri = (struct Triangles*) malloc(sizeof(struct Triangles));
    if(tri == NULL){
      printf("Error: Out of Memory\n");
      return -1;
    }
    tri->x1 = x1;
    tri->y1 = y1;
    tri->x2 = x2;
    tri->y2 = y2;
    tri->x3 = x3;
    tri->y3 = y3;
    tri->fill = getFill(fill);
    if(insertE(tri, type, color) == 0){
      return 0;
    } else {
     return -1;
    }
  }
  return -1;
}


int assignText(int cx, int cy, int cw, int ch, int cAlign, char* cText, int color, int boarder, int font, int fontSize, int fontStyle, int padding){
  struct Texts* text;
  text = (struct Texts*) malloc(sizeof(struct Texts));
  if(text == NULL){
    printf("Error: Out of Memory\n");
    return -1;
  }
  text->x1 = cx;
  text->x2 = cw;
  text->y1 = cy;
  text->y2 = ch;
  text->align = cAlign;
  text->text = cText;
  text->font = font;
  text->fontStyle = fontStyle;
  if(padding < 0){
    text->padding = 0;
  } else {
    text->padding = padding;
  }
  if(boarder == 0){
    text->boarder = 0;
  } else {
    text->boarder = 1;
  }
  if(fontSize <= 0){
    text->fontSize = 1;
  } else {
    text->fontSize = fontSize;
  }
  if(insertE(text, Text, color) == 0){
    return 0;
  } else {
    return -1;
  }
}

void deletePoly(struct Polygons* poly){
  if(poly == NULL){
    return;
  }
  struct Vertex* vertex = poly->vertices;
  struct Vertex* next;
  while(vertex != NULL){
    next = vertex->next;
    free(vertex);
    vertex = next;
  }
  free(poly);
}

int assignPoly(int n, TERM x, TERM y, int color, int fill){
  struct Polygons* poly;
  struct Vertex* vertex;
  struct Vertex* last;
  TERM curX;
  TERM curY;
  TERM remX;
  TERM remY;
  int valX;
  int valY;
  poly = (struct Polygons*) malloc(sizeof(struct Polygons));
  if(poly == NULL){
    printf("Error: Out of Memory\n");
    return -1;
  }
  poly->fill = getFill(fill);
  curX = x;
  curY = y;
  for(int i = 0; i < n; i++){
    vertex = (struct Vertex*)malloc(sizeof(struct Vertex));
    if(vertex == NULL){
      deletePoly(poly);
      return -1;
    }
    vertex->next = NULL;
    curX = picat_get_car(x);
    curY = picat_get_car(y);
    x = picat_get_cdr(x);
    y = picat_get_cdr(y);
    if(!picat_is_integer(curX) || !picat_is_integer(curY)){
      deletePoly(poly);
      free(vertex);
      return -1;
    } 
    valX = (int) picat_get_integer(curX);
    valY = (int) picat_get_integer(curY);
    vertex->x = valX;
    vertex->y = valY;
    if( i == 0){
      poly->vertices = vertex;
    } else {
      last->next = vertex;
    }
    last = vertex;
    curX = x;
    curY = y;
  }
  if(insertE(poly, Polygon, color) == 0){
    return 0;
  } else {
    return -1;
  }
}


int c_arc(){
  TERM x1 	= picat_get_call_arg(1,8);
  TERM y1 	= picat_get_call_arg(2,8);
  TERM x2 	= picat_get_call_arg(3,8);
  TERM y2 	= picat_get_call_arg(4,8);
  TERM color    = picat_get_call_arg(5,8);
  TERM angle1 	= picat_get_call_arg(6,8);
  TERM angle2	= picat_get_call_arg(7,8);
  TERM fill 	= picat_get_call_arg(8,8);
  int ix1 = 0;
  int iy1 = 0;
  int ix2 = 0;
  int iy2 = 0;
  long cAngle1 = 0;
  long cAngle2 = 0;
  char* buffer;
  if(!picat_is_integer(x1) || !picat_is_integer(y1) || !picat_is_integer(x2) || !picat_is_integer(y2) || !picat_is_integer(angle1) || !picat_is_integer(angle2) || !picat_is_integer(fill)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    return PICAT_ERROR;
  }

  ix1 = (int) picat_get_integer(x1);
  ix2 = (int) picat_get_integer(x2);
  iy1 = (int) picat_get_integer(y1);
  iy2 = (int) picat_get_integer(y2);
  cAngle1 = picat_get_integer(angle1);
  cAngle2 = picat_get_integer(angle2);
  long cFill = picat_get_integer(fill);
  buffer = picat_get_atom_name(color);
  int iColor = getColorValue(buffer);
  if(assignArc(ix1+ix2/2,ix2,iy1+iy2/2,iy2,cAngle1,cAngle2,iColor,(int)cFill,Circle) != 0)
    return PICAT_ERROR; 
  return PICAT_TRUE;
}

int c_triangle(){
  TERM x1 	= picat_get_call_arg(1,8);
  TERM y1 	= picat_get_call_arg(2,8);
  TERM x2 	= picat_get_call_arg(3,8);
  TERM y2 	= picat_get_call_arg(4,8);
  TERM x3 	= picat_get_call_arg(5,8);
  TERM y3	= picat_get_call_arg(6,8);
  TERM color	= picat_get_call_arg(7,8);
  TERM fill 	= picat_get_call_arg(8,8);
  int ix1 = 0;
  int ix2 = 0;
  int ix3 = 0;
  int iy1 = 0;
  int iy2 = 0;
  int iy3 = 0;
  char* col;
  if(!picat_is_integer(x1) || !picat_is_integer(y1)|| !picat_is_integer(x2) || !picat_is_integer(y2)||!picat_is_integer(x3) ||!picat_is_integer(y3)||!picat_is_integer(fill)){
    printf("Error: Expected interger value\n");
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    printf("Error: Expected atom\n");
    return PICAT_ERROR;
  }
  ix1 = (int) picat_get_integer(x1);
  ix2 = (int) picat_get_integer(x2);
  ix3 = (int) picat_get_integer(x3);
  iy1 = (int) picat_get_integer(y1);
  iy2 = (int) picat_get_integer(y2);
  iy3 = (int) picat_get_integer(y3);
  col = picat_get_atom_name(color);
  long cFill = picat_get_integer(fill);
  int iColor = getColorValue(col);
  if(assignTri(ix1,iy1,ix2,iy2,ix3,iy3,(int)cFill,Triangle,iColor)!= 0)
    return PICAT_ERROR;
  return PICAT_TRUE;
}

int c_oval(){
  TERM x1 = picat_get_call_arg(1,6);
  TERM y1 = picat_get_call_arg(2,6);
  TERM x2 = picat_get_call_arg(3,6);
  TERM y2 = picat_get_call_arg(4,6);
  TERM color = picat_get_call_arg(5,6);
  TERM fill = picat_get_call_arg(6,6);
  int ix1 = 0;
  int ix2 = 0;
  int iy1 = 0;
  int iy2 = 0;
  char* buffer;
  if(!picat_is_integer(x1)||!picat_is_integer(y1)||!picat_is_integer(x2)||!picat_is_integer(y2) || !picat_is_integer(fill)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    return PICAT_ERROR;
  }
  ix1 = (int) picat_get_integer(x1);
  ix2 = (int) picat_get_float(x2);
  iy1 = (int) picat_get_float(y1);
  iy2 = (int) picat_get_integer(y2);
  buffer = picat_get_atom_name(color);
  long cFill = picat_get_integer(fill);
  int iColor = getColorValue(buffer);
  if(assignArc(ix1+ix2/2,ix2,iy1+iy2/2,iy2,0,360,iColor,(int)cFill,Circle)!=0)
    return PICAT_ERROR;
  return PICAT_TRUE;
}

int c_circle(){
  TERM x1 = picat_get_call_arg(1,5);
  TERM y1 = picat_get_call_arg(2,5);
  TERM x2 = picat_get_call_arg(3,5);
  TERM color = picat_get_call_arg(4,5);
  TERM fill = picat_get_call_arg(5,5);
  int cx = 0;
  int cy = 0;
  int cx2= 0;
  char* buffer;
  if (!picat_is_integer(x1)||!picat_is_integer(y1)||!picat_is_integer(x2)|| !picat_is_integer(fill)){
    return PICAT_ERROR;   
  }
  if(!picat_is_atom(color)){
    return PICAT_ERROR;
  }
  cx  = (int) picat_get_integer(x1);
  cy  = (int) picat_get_integer(y1);
  cx2 = (int) picat_get_integer(x2);
  buffer = picat_get_atom_name(color);
  long cFill = picat_get_integer(fill);
  int iColor = getColorValue(buffer);
  if(assignArc((int)cx+cx2/2,(int)cx2,(int)cy+cx2/2,(int)cx2,0,360,iColor,(int)cFill,Circle)!= 0)
    return PICAT_ERROR; 
  return PICAT_TRUE;
}

int c_line(){
  TERM x1 = picat_get_call_arg(1,5);
  TERM y1 = picat_get_call_arg(2,5);
  TERM x2 = picat_get_call_arg(3,5);
  TERM y2 = picat_get_call_arg(4,5);
  TERM color = picat_get_call_arg(5,5);
  int ix1 = 0;
  int iy1 = 0;
  int ix2 = 0;
  int iy2 = 0;
  char* buffer;
  int iColor;
  if(!picat_is_integer(x1) ||!picat_is_integer(y1) ||!picat_is_integer(x2) ||!picat_is_integer(y2)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    return PICAT_ERROR;
  }
  
  ix1 = (int) picat_get_integer(x1);
  iy1 = (int) picat_get_integer(y1);
  ix2 = (int) picat_get_integer(x2);
  iy2 = (int) picat_get_integer(y2);
  buffer = picat_get_atom_name(color);
  iColor = getColorValue(buffer);
  if(assignLine(ix1,ix2,iy1, iy2, Line, iColor) != 0)
    return PICAT_ERROR; 
  return PICAT_TRUE;
}

int c_rectangle(){
  TERM xMin = picat_get_call_arg(1,6);
  TERM yMin = picat_get_call_arg(2,6);
  TERM xMax = picat_get_call_arg(3,6);
  TERM yMax = picat_get_call_arg(4,6);
  TERM color = picat_get_call_arg(5,6);
  TERM fill = picat_get_call_arg(6,6);
  int xLow = 0;
  int xHigh = 0;
  int yLow = 0;
  int yHigh = 0;
  char* buffer;
  if(!picat_is_integer(xMin) ||!picat_is_integer(yMin) ||!picat_is_integer(xMax) ||!picat_is_integer(yMax)||!picat_is_integer(fill)){
    printf("Error: Expected an integer value\n");
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    printf("Error: Expected a string value\n");
    return PICAT_ERROR;
  }
  xLow  = (int) picat_get_integer(xMin);
  xHigh = (int) picat_get_integer(xMax);
  yLow  = (int) picat_get_integer(yMin);
  yHigh = (int) picat_get_integer(yMax);
  long cFill = picat_get_integer(fill);
  buffer = picat_get_atom_name(color);
  int iColor = getColorValue(buffer);
  if(assignRect(xLow,xHigh+xLow,yLow,yLow+yHigh,iColor,(int)cFill,2) != 0)
    return PICAT_ERROR;
  return PICAT_TRUE;
}


int c_star(){
  TERM x = 		picat_get_call_arg(1,11);
  TERM y = 		picat_get_call_arg(2,11);
  TERM w = 		picat_get_call_arg(3,11);
  TERM h = 		picat_get_call_arg(4,11);
  TERM n = 		picat_get_call_arg(5,11);
  TERM centerX = 	picat_get_call_arg(6,11);
  TERM centerY = 	picat_get_call_arg(7,11);
  TERM angle = 		picat_get_call_arg(8,11);
  TERM diameter = 	picat_get_call_arg(9,11);
  TERM color =		picat_get_call_arg(10,11);
  TERM fill = 		picat_get_call_arg(11,11);

  if(!picat_is_integer(x) || !picat_is_integer(y) || !picat_is_integer(w) || !picat_is_integer(h) || !picat_is_integer(n) || !picat_is_integer(centerX) ||!picat_is_integer(centerY) || !picat_is_integer(angle) || !picat_is_integer(diameter) || !picat_is_integer(fill)){
    printf("Error: Integer Expected\n");
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    printf("Error: Atom Expected\n");
    return PICAT_ERROR;
  }
  long cx = picat_get_integer(x);
  long cy = picat_get_integer(y);
  long cw = picat_get_integer(w);
  long ch = picat_get_integer(h);
  long cn = picat_get_integer(n);
  long cCenterX = picat_get_integer(centerX);
  long cCenterY = picat_get_integer(centerY);
  long cAngle = picat_get_integer(angle);
  long cDiameter = picat_get_integer(diameter);
  long cFill = picat_get_integer(fill);
  char* buffer = picat_get_atom_name(color);

  int iColor = getColorValue(buffer);
  if(assignStar(cx,cy,cw,ch,cn,cCenterX,cCenterY,cAngle,cDiameter,iColor,cFill)!= 0)
    return PICAT_ERROR;
  return PICAT_TRUE;
}


int c_polygon(){
  TERM n = 	picat_get_call_arg(1,5);
  TERM x = 	picat_get_call_arg(2,5);
  TERM y = 	picat_get_call_arg(3,5);
  TERM color = 	picat_get_call_arg(4,5);
  TERM fill = 	picat_get_call_arg(5,5);
  if(!picat_is_integer(n) || !picat_is_integer(fill)){
    printf("Error: Integer Expected\n");
    return PICAT_ERROR;
  }
  if(!picat_is_list(x) ||!picat_is_list(y)){
    printf("Error: List Expected\n");
    return PICAT_ERROR;
  }
  if(!picat_is_atom(color)){
    printf("Error: Atom Expected\n");
    return PICAT_ERROR;
  }
  int cn = (int) picat_get_integer(n);
  int cFill = (int) picat_get_integer(fill);
  char* buffer = picat_get_atom_name(color);
  int iColor = getColorValue(buffer);
  if(assignPoly(cn, x, y, iColor, cFill) != 0)
    return PICAT_ERROR;
  return PICAT_TRUE;
}

int c_text(){
  TERM x = 		picat_get_call_arg(1,11);
  TERM y = 		picat_get_call_arg(2,11);
  TERM w = 		picat_get_call_arg(3,11);
  TERM h = 		picat_get_call_arg(4,11);
  TERM text = 		picat_get_call_arg(5,11);
  TERM align =		picat_get_call_arg(6,11);
  TERM color =	 	picat_get_call_arg(7,11);
  TERM font =		picat_get_call_arg(8,11);
  TERM fontSize = 	picat_get_call_arg(9,11);
  TERM fontStyle =	picat_get_call_arg(10,11);
  TERM padding =	picat_get_call_arg(11,11);
  if(!picat_is_integer(x) || !picat_is_integer(y) || !picat_is_integer(w) || !picat_is_integer(h) || !picat_is_integer(padding) || !picat_is_integer(fontSize)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(text) || !picat_is_atom(color) || !picat_is_atom(align) || !picat_is_atom(font) || !picat_is_atom(fontStyle)){
    return PICAT_ERROR;
  }
  int cx = (int) picat_get_integer(x);
  int cy = (int) picat_get_integer(y);
  int cw = (int) picat_get_integer(w);
  int ch = (int) picat_get_integer(h);
  int cPadding = (int) picat_get_integer(padding);
  int cFontSize = (int) picat_get_integer(fontSize);
  char* cText = picat_get_atom_name(text);

  char* buffer = picat_get_atom_name(color);
  int iColor = getColorValue(buffer);

  char* sAlign = picat_get_atom_name(align);
  int cAlign = getAlignValue(sAlign);

  char* sFont = picat_get_atom_name(font);
  int cFont = getFontValue(sFont);

  char* sStyle = picat_get_atom_name(fontStyle);
  int cStyle = getFontStyleValue(sStyle);
  
  if(assignText(cx, cy, cw, ch, cAlign, cText, iColor, 1, cFont, cFontSize, cStyle, cPadding) == 0){
    return PICAT_TRUE;
  } else {
    return PICAT_ERROR;
  }
}

int c_label(){
  TERM x = 		picat_get_call_arg(1,10);
  TERM y = 		picat_get_call_arg(2,10);
  TERM w = 		picat_get_call_arg(3,10);
  TERM h = 		picat_get_call_arg(4,10);
  TERM text = 		picat_get_call_arg(5,10);
  TERM align =		picat_get_call_arg(6,10);
  TERM color = 		picat_get_call_arg(7,10);
  TERM font = 		picat_get_call_arg(8,10);
  TERM fontSize = 	picat_get_call_arg(9,10);
  TERM fontStyle =	picat_get_call_arg(10,10);
  if(!picat_is_integer(x) || !picat_is_integer(y) || !picat_is_integer(w) || !picat_is_integer(h)|| !picat_is_integer(fontSize)){
    return PICAT_ERROR;
  }
  if(!picat_is_atom(text) || !picat_is_atom(color) || !picat_is_atom(align)|| !picat_is_atom(font) || !picat_is_atom(fontStyle)){
    return PICAT_ERROR;
  }
  int cx = (int) picat_get_integer(x);
  int cy = (int) picat_get_integer(y);
  int cw = (int) picat_get_integer(w);
  int ch = (int) picat_get_integer(h);
  int cFontSize = (int) picat_get_integer(fontSize);

  char* cText = picat_get_atom_name(text);

  char* buffer = picat_get_atom_name(color);
  int iColor = getColorValue(buffer);

  char* sAlign = picat_get_atom_name(align);
  int cAlign = getAlignValue(sAlign);

  char* sFont = picat_get_atom_name(font);
  int cFont = getFontValue(sFont);

  char* sStyle = picat_get_atom_name(fontStyle);
  int cStyle = getFontStyleValue(sStyle);

  if(assignText(cx, cy, cw, ch, cAlign, cText, iColor, 0, cFont,cFontSize,cStyle, 0) == 0){
    return PICAT_TRUE;
  } else {
    return PICAT_ERROR;
  }
}
int c_canvas()
{
  TERM x = picat_get_call_arg(1,2);
  TERM y = picat_get_call_arg(2,2);

  if(!picat_is_integer(x) || !picat_is_integer(y)){
    return PICAT_ERROR;
  }
  long cX =(long)  picat_get_integer(x) / 1.75;
  long cY =(long)  picat_get_integer(y) / 2.12;

  snprintf(canvasSize, 100, "%lix%li", cX, cY);
  Ihandle *dlg, *cnvs;
  IupOpen(NULL, NULL);

  cnvs = IupCanvas( NULL );
  IupSetCallback(cnvs, "ACTION",( Icallback )redraw_cb );
  IupSetAttribute(cnvs, "SIZE", canvasSize);

  dlg = IupDialog( IupVbox( cnvs, NULL ) );
  IupSetAttribute(dlg, "TITLE", "Picat 2.4" );
  IupSetAttribute(dlg, "MARGIN", "10x10");
  IupMap( dlg );

  //cdInitContextPlus();
  //cdUseContextPlus(1);

  cdcanvas = cdCreateCanvas( CD_IUP, cnvs );
  cdCanvasSetAttribute(cdcanvas, "ANTIALIAS", "0");
  //int *width = malloc(sizeof(int));
  //int *height = malloc(sizeof(int));
  //cdCanvasGetSize(cdcanvas, width, height, NULL, NULL);
  //printf("height %d %d", *width, *height);   
  IupShowXY( dlg, IUP_CENTER, IUP_CENTER );
  IupMainLoop();
  IupClose();  
  return PICAT_TRUE;
}

