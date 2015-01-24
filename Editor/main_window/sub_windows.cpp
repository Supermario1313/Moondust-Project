/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014-2015 Vitaly Novichkov <admin@wohlnet.ru>
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

#include <common_features/graphicsworkspace.h>
#include <common_features/themes.h>

#include <ui_mainwindow.h>
#include <mainwindow.h>

//Edit NPC
NpcEdit *MainWindow::createNPCChild()
{
    QMdiSubWindow *npcWindow = new QMdiSubWindow(ui->centralWidget);
    NpcEdit *child = new NpcEdit(&configs, npcWindow);
    npcWindow->setWidget(child);
    npcWindow->setAttribute(Qt::WA_DeleteOnClose);

    QMdiSubWindow * npcWindowP = ui->centralWidget->addSubWindow(npcWindow);
    npcWindowP->setAttribute(Qt::WA_DeleteOnClose);

    npcWindowP->setWindowIcon(Themes::icon(Themes::npc_16));

    npcWindowP->setGeometry(
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().width()/4),
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().height()/4),
                 520,640);

    ui->centralWidget->updateGeometry();

    connect(npcWindow, SIGNAL(destroyed(QObject*)), this, SLOT(recordRemovedWindow(QObject*)));

    return child;
}



//Edit LEVEL
LevelEdit *MainWindow::createLvlChild()
{
    QMdiSubWindow *levelWindow = new QMdiSubWindow(ui->centralWidget);
    LevelEdit *child = new LevelEdit(levelWindow);
    connect(child, SIGNAL(forceReload()), this, SLOT(on_actionReload_triggered()));

    levelWindow->setWidget(child);
    levelWindow->setAttribute(Qt::WA_DeleteOnClose);

    child->setAcceptDrops(true);

    levelWindow->setAcceptDrops(true);

    QMdiSubWindow * levelWindowP = ui->centralWidget->addSubWindow(levelWindow);
    levelWindowP->setAttribute(Qt::WA_DeleteOnClose);
    levelWindowP->setAcceptDrops(true);

    levelWindowP->setGeometry(
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().width()/4),
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().height()/4),
                             860, 680);
    levelWindowP->setWindowIcon(Themes::icon(Themes::level_16));
    levelWindowP->updateGeometry();
    levelWindowP->update();
    ui->centralWidget->updateGeometry();

    GraphicsWorkspace* gr = static_cast<GraphicsWorkspace *>(child->getGraphicsView());
    connect(gr, SIGNAL(zoomValueChanged(QString)), zoom, SLOT(setText(QString)));

    connect(levelWindow, SIGNAL(destroyed(QObject*)), this, SLOT(recordRemovedWindow(QObject*)));

    return child;
}

//Edit WORLD
WorldEdit *MainWindow::createWldChild()
{
    QMdiSubWindow *worldWindow = new QMdiSubWindow(ui->centralWidget);
    WorldEdit *child = new WorldEdit(worldWindow);
    connect(child, SIGNAL(forceReload()), this, SLOT(on_actionReload_triggered()));

    worldWindow->setWidget(child);
    worldWindow->setAttribute(Qt::WA_DeleteOnClose);

    QMdiSubWindow * levelWindowP = ui->centralWidget->addSubWindow(worldWindow);
    levelWindowP->setAttribute(Qt::WA_DeleteOnClose);

    levelWindowP->setGeometry(
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().width()/4),
                (ui->centralWidget->subWindowList().size()*20)%(ui->centralWidget->size().height()/4),
                             860, 680);
    levelWindowP->setWindowIcon(Themes::icon(Themes::world_16));
    levelWindowP->updateGeometry();
    levelWindowP->update();
    ui->centralWidget->updateGeometry();

    GraphicsWorkspace* gr = static_cast<GraphicsWorkspace *>(child->getGraphicsView());
    connect(gr, SIGNAL(zoomValueChanged(QString)), zoom, SLOT(setText(QString)));

    connect(worldWindow, SIGNAL(destroyed(QObject*)), this, SLOT(recordRemovedWindow(QObject*)));

    return child;
}



//Common functions

int MainWindow::activeChildWindow()
{
    if (QMdiSubWindow *activeSubWindow = ui->centralWidget->activeSubWindow())
    {
        return activeChildWindow(activeSubWindow);
    }

    return 0;
}

int MainWindow::activeChildWindow(QMdiSubWindow *wnd)
{
    if(QString(wnd->widget()->metaObject()->className())==LEVEL_EDIT_CLASS)
        return 1;
    if(QString(wnd->widget()->metaObject()->className())==NPC_EDIT_CLASS)
        return 2;
    if(QString(wnd->widget()->metaObject()->className())==WORLD_EDIT_CLASS)
        return 3;
}

NpcEdit *MainWindow::activeNpcEditWin()
{
    if (QMdiSubWindow *activeSubWindow = ui->centralWidget->activeSubWindow())
        return qobject_cast<NpcEdit *>(activeSubWindow->widget());
    return 0;
}

LevelEdit *MainWindow::activeLvlEditWin()
{
    if (QMdiSubWindow *activeSubWindow = ui->centralWidget->activeSubWindow())
        return qobject_cast<LevelEdit *>(activeSubWindow->widget());
    return 0;
}

WorldEdit *MainWindow::activeWldEditWin()
{
    if (QMdiSubWindow *activeSubWindow = ui->centralWidget->activeSubWindow())
        return qobject_cast<WorldEdit *>(activeSubWindow->widget());
    return 0;
}



QMdiSubWindow *MainWindow::findOpenedFileWin(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
    LevelEdit *ChildWindow0;
    NpcEdit *ChildWindow2;
    WorldEdit *ChildWindow3;

    foreach (QMdiSubWindow *window, ui->centralWidget->subWindowList())
    {
        if(QString(window->widget()->metaObject()->className())==LEVEL_EDIT_CLASS)
        {
        ChildWindow0 = qobject_cast<LevelEdit *>(window->widget());
        if (ChildWindow0->currentFile() == canonicalFilePath)
            return window;
        }
        else if(QString(window->widget()->metaObject()->className())==NPC_EDIT_CLASS)
        {
        ChildWindow2 = qobject_cast<NpcEdit *>(window->widget());
        if (ChildWindow2->currentFile() == canonicalFilePath)
            return window;
        }
        else if(QString(window->widget()->metaObject()->className())==WORLD_EDIT_CLASS)
        {
        ChildWindow3 = qobject_cast<WorldEdit *>(window->widget());
        if (ChildWindow3->currentFile() == canonicalFilePath)
            return window;
        }
    }
    return 0;
}


void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    ui->centralWidget->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}


void MainWindow::close_sw()
{
    if(ui->centralWidget->subWindowList().size()>0)
        ui->centralWidget->activeSubWindow()->close();
}


int MainWindow::subWins()
{
    return ui->centralWidget->subWindowList().size();
}



void MainWindow::SWCascade()
{
    if(GlobalSettings::MainWindowView!=QMdiArea::SubWindowView)
        setSubView(); // Switch into SubWindow mode on call this menuitem
    ui->centralWidget->cascadeSubWindows();

    foreach(QMdiSubWindow * w, ui->centralWidget->subWindowList())
    {
        if(
                (QString(w->widget()->metaObject()->className())==LEVEL_EDIT_CLASS)||
                (QString(w->widget()->metaObject()->className())==WORLD_EDIT_CLASS)
           ) w->setGeometry(w->x(), w->y(), 860, 680);
    }

}

void MainWindow::SWTile()
{
    if(GlobalSettings::MainWindowView!=QMdiArea::SubWindowView)
        setSubView(); // Switch into SubWindow mode on call this menuitem
    ui->centralWidget->tileSubWindows();
}

void MainWindow::setSubView()
{
    GlobalSettings::MainWindowView = QMdiArea::SubWindowView;
    ui->centralWidget->setViewMode(GlobalSettings::MainWindowView);
    updateWindowMenu();
}

void MainWindow::setTabView()
{
    GlobalSettings::MainWindowView = QMdiArea::TabbedView;
    ui->centralWidget->setViewMode(GlobalSettings::MainWindowView);
    updateWindowMenu();
}

