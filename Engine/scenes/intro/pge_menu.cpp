/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pge_menu.h"
#include <graphics/window.h>
#include <fontman/font_manager.h>

PGE_Menu::PGE_Menu()
{
    _itemsOnScreen=5;
    _currentItem = 0;
    _line=0;
    _offset=0;
    arrowUpViz=false;
    arrowDownViz=false;
    _EndSelection=false;
    _accept=false;
    menuRect = QRect(260,380, 350, 30);
}

PGE_Menu::~PGE_Menu()
{
    clear();
}

void PGE_Menu::addMenuItem(QString value, QString title)
{
    PGE_Menuitem item;
    item.value = value;
    item.title = (title.isEmpty() ? value : title);
    item.textTexture = FontManager::TextToTexture(item.title,
                                                  QRect(0,0, menuRect.width(), menuRect.height()),
                                                  Qt::AlignLeft | Qt::AlignVCenter );
    _items.push_back(item);
}

void PGE_Menu::clear()
{
    for(int i=0;i<_items.size();i++)
    {
        glDisable(GL_TEXTURE_2D);
        glDeleteTextures(1, &_items[i].textTexture );
    }
    _items.clear();
    reset();
}

void PGE_Menu::selectUp()
{
    _currentItem--;
    _line--;

    if(_line<0)
    {
        _offset--;
        _line=0;
        if(_offset<0) _offset=0;
    }

    if(_currentItem<0)
    {
        _currentItem=_items.size()-1;
        _line = _itemsOnScreen-1;
        _offset = (_items.size()>_itemsOnScreen)? _items.size()-_itemsOnScreen : 0;
    }
}

void PGE_Menu::selectDown()
{
    _currentItem++;
    _line++;
    if(_line > _itemsOnScreen-1)
    {
        _offset++;
        _line = _itemsOnScreen-1;
    }

    if(_currentItem >= _items.size())
    {
        _currentItem = 0;
        _line=0;
        _offset=0;
    }
}

void PGE_Menu::scrollUp()
{
    if(_items.size()<=_itemsOnScreen) return;
    if(_offset>0)
    {
        _offset--;
        _currentItem--;
    }
}

void PGE_Menu::scrollDown()
{
    if(_items.size()<=_itemsOnScreen) return;

    if(_offset < (_items.size()-_itemsOnScreen))
    {
        _offset++;
        _currentItem++;
    }
}

void PGE_Menu::acceptItem()
{
    _EndSelection=true;
    _accept=true;
}

void PGE_Menu::rejectItem()
{
    _EndSelection=true;
    _accept=false;
}

void PGE_Menu::setItemsNumber(int q)
{
    if(q>0)
        _itemsOnScreen = q;
    else
        _itemsOnScreen = 5;
}

void PGE_Menu::sort()
{
    while(1)
    {
        bool swaped=false;
        for(int i=0; i<_items.size()-1; i++)
        {
            if(namefileLessThan(_items[i], _items[i+1]))
            {
                _items.swap(i, i+1);

                if(_currentItem==i)
                    _currentItem=i+1;
                else
                if(_currentItem==i+1)
                    _currentItem=i;

                swaped=true;
            }
        }
        if(!swaped) break;
    }
    autoOffset();
}

bool PGE_Menu::namefileLessThan(const PGE_Menuitem &d1, const PGE_Menuitem &d2)
{
    return (QString::compare(d1.title, d2.title, Qt::CaseInsensitive)>0); // sort by title
}

bool PGE_Menu::isSelected()
{
    return _EndSelection;
}

bool PGE_Menu::isAccepted()
{
    return _accept;
}



void PGE_Menu::reset()
{
    _EndSelection=false;
    _accept=false;
    _offset=0;
    _line=0;
    _currentItem=0;
}

void PGE_Menu::resetState()
{
    _EndSelection=false;
    _accept=false;
}

void PGE_Menu::setMouseHoverPos(int x, int y)
{
    int item = findItem(x,y);
    if(item<0) return;
    setCurrentItem(item);
}

void PGE_Menu::setMouseClickPos(int x, int y)
{
    int item = findItem(x,y);
    if(item<0) return;
    acceptItem();
}

int PGE_Menu::findItem(int x, int y)
{
    if(x>menuRect.right()) return -1;
    if(x<menuRect.left())  return -1;
    if(y<menuRect.top())  return -1;
    if(y>menuRect.top()+menuRect.height()*_itemsOnScreen) return -1;

    int pos = menuRect.y();
    for(int i=0; (i<_itemsOnScreen) && (i<_items.size()); i++)
    {
        if( (y > pos) && ( y<(pos+menuRect.height()) ) )
        {
            return _offset+i;
        }
        pos+=menuRect.height();
    }

    return -1;
}


const PGE_Menuitem PGE_Menu::currentItem()
{
    if(_items.size()>0)
        return _items[_currentItem];
    else
    {
        PGE_Menuitem dummy;
        dummy.title="";
        dummy.value="";
        return dummy;
    }
}

int PGE_Menu::currentItemI()
{
    return _currentItem;
}

void PGE_Menu::setCurrentItem(int i)
{
    //If no out of range
    if((i>=0)&&(i<_items.size()))
    {
        _currentItem=i;
        autoOffset();
    }
}

int PGE_Menu::line()
{
    return _line;
}

void PGE_Menu::setLine(int ln)
{
    if((ln>=0) && (ln<_itemsOnScreen))
        _line = ln;
    else
        _line = _itemsOnScreen/2;
}

int PGE_Menu::offset()
{
    return _offset;
}

void PGE_Menu::setOffset(int of)
{
    if((of>=0)&&(of< (_items.size()-_itemsOnScreen)))
    {
       _offset=of;
       _line = _currentItem-of;
       _line = ((_line>0)?
                   ((_line<_itemsOnScreen)?_line : _itemsOnScreen)
                   :0 );
       autoOffset();
    }
    else
    {
        autoOffset();
    }
}

///
/// \brief PGE_Menu::autoOffset
/// Automatically sets offset and line number values
void PGE_Menu::autoOffset()
{
    if(_items.size()<=_itemsOnScreen)
    {
        _offset=0;
        _line=_currentItem;
        return;
    }

    if(_currentItem-_itemsOnScreen > _offset)
    {
        _offset=_currentItem-_itemsOnScreen+1;
        _line=_itemsOnScreen-1;
    }
    else
    if(_currentItem > (_offset+_itemsOnScreen-1))
    {
        _offset=_currentItem-_itemsOnScreen+1;
        _line=_itemsOnScreen-1;
    }
    else
    {
        _line=_currentItem-_offset;
    }
}



void PGE_Menu::setPos(int x, int y)
{
    menuRect.setX(x);
    menuRect.setY(y);
}

void PGE_Menu::setPos(QPoint p)
{
    menuRect.setTopLeft(p);
}

void PGE_Menu::setSize(int w, int h)
{
    menuRect.setWidth(w);
    menuRect.setHeight(h);
}

void PGE_Menu::setSize(QSize s)
{
    menuRect.setSize(s);
}

QRect PGE_Menu::rect()
{
    return menuRect;
}

void PGE_Menu::render()
{
    //Show scrollers
    if(_items.size()>_itemsOnScreen)
    {
        if(_offset > 0)
        {
            glDisable(GL_TEXTURE_2D);
            glColor4f( 0.f, 1.f, 0.f, 1.0f);
            int posX = menuRect.x()+menuRect.width()/2;
            int posY = menuRect.y()-20;
            glBegin( GL_QUADS );
                glVertex2f( posX, posY);
                glVertex2f( posX+10, posY);
                glVertex2f( posX+10, posY+10);
                glVertex2f( posX, posY+10);
            glEnd();
        }

        if(_offset< (_items.size()-_itemsOnScreen))
        {
            glDisable(GL_TEXTURE_2D);
            glColor4f( 0.f, 1.f, 0.f, 1.0f);
            int posX = menuRect.x()+menuRect.width()/2;
            int posY = menuRect.y()+menuRect.height()*_itemsOnScreen;
            glBegin( GL_QUADS );
                glVertex2f( posX, posY);
                glVertex2f( posX+10, posY);
                glVertex2f( posX+10, posY+10);
                glVertex2f( posX, posY+10);
            glEnd();
        }
    }

    for(int i=_offset, j=0; i<_offset+_itemsOnScreen && i<_items.size(); i++, j++ )
    {
        int xPos = menuRect.x();
        int yPos = menuRect.y()+ j*menuRect.height();

        if(i==_currentItem)
        {
            glDisable(GL_TEXTURE_2D);
            glColor4f( 1.f, 1.f, 0.f, 1.0f);
            glBegin( GL_QUADS );
                glVertex2f( xPos-20,      yPos + (menuRect.height()/2)-5 );
                glVertex2f( xPos-20+10,   yPos + (menuRect.height()/2)-5 );
                glVertex2f( xPos-20+10,10+yPos + (menuRect.height()/2)-5 );
                glVertex2f( xPos-20,   10+yPos + (menuRect.height()/2)-5 );
            glEnd();
        }

        FontManager::SDL_string_render2D(xPos,yPos,
                                        &_items[i].textTexture);
    }
}

PGE_Menuitem::PGE_Menuitem()
{
    this->title = "";
    this->value = "";
    this->textTexture = 0;
}

PGE_Menuitem::~PGE_Menuitem()
{}

PGE_Menuitem::PGE_Menuitem(const PGE_Menuitem &_it)
{
    this->title = _it.title;
    this->value = _it.value;
    this->textTexture = _it.textTexture;
}


