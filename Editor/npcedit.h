#ifndef NPCEDIT_H
#define NPCEDIT_H

#include <QWidget>
#include <QFile>
#include "npc_filedata.h"

namespace Ui {
class npcedit;
}

class npcedit : public QWidget
{
    Q_OBJECT
    
public:
    explicit npcedit(QWidget *parent = 0);
    ~npcedit();

    void newFile();
    bool loadFile(const QString &fileName, NPCConfigFile FileData);
    NPCConfigFile NpcData;

    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();

    void on_en_GFXOffsetX_clicked();

    void on_en_GFXOffsetY_clicked();

    void on_En_GFXw_clicked();

    void on_En_GFXh_clicked();

    void on_En_Frames_clicked();

    void on_En_Framespeed_clicked();

    void on_En_Framestyle_clicked();

    void on_En_IsForeground_clicked();

    void on_En_GrabSide_clicked();

    void on_En_GrabTop_clicked();

    void on_En_JumpHurt_clicked();

    void on_En_DontHurt_clicked();

    void on_En_Score_clicked();

    void on_En_NoEat_clicked();

    void on_En_NoFireball_clicked();

    void on_En_NoIceball_clicked();

    void on_En_Width_clicked();

    void on_En_Height_clicked();

    void on_En_Speed_clicked();

    void on_En_PlayerBlock_clicked();

    void on_En_PlayerBlockTop_clicked();

    void on_En_NPCBlock_clicked();

    void on_En_NPCBlockTop_clicked();

    void on_En_NoBlockCollision_clicked();

    void on_En_NoGravity_clicked();

    void on_En_TurnCliff_clicked();

    void on_ResetNPCData_clicked();

    void on_GFXOffSetX_valueChanged(int arg1);

    void on_GFXOffSetY_valueChanged(int arg1);

    void on_GFXw_valueChanged(int arg1);

    void on_GFXh_valueChanged(int arg1);

    void on_Frames_valueChanged(int arg1);

    void on_Framespeed_valueChanged(int arg1);

    void on_FrameStyle_currentIndexChanged(int index);

    void on_IsForeground_stateChanged(int arg1);

    void on_GrabSide_stateChanged(int arg1);

    void on_GrabTop_stateChanged(int arg1);

    void on_JumpHurt_stateChanged(int arg1);

    void on_DontHurt_stateChanged(int arg1);

    void on_Score_currentIndexChanged(int index);

    void on_NoEat_stateChanged(int arg1);

    void on_NoFireball_stateChanged(int arg1);

    void on_NoIceball_stateChanged(int arg1);

    void on_Width_valueChanged(int arg1);

    void on_Height_valueChanged(int arg1);

    void on_Speed_valueChanged(double arg1);

    void on_PlayerBlock_stateChanged(int arg1);

    void on_PlayerBlockTop_stateChanged(int arg1);

    void on_NPCBlock_stateChanged(int arg1);

    void on_NPCBlockTop_stateChanged(int arg1);

    void on_NoBlockCollision_stateChanged(int arg1);

    void on_NoGravity_stateChanged(int arg1);

    void on_TurnCliff_stateChanged(int arg1);

private:
    Ui::npcedit *ui;

    NPCConfigFile StartNPCData;
    bool isModyfied;

    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    void setDataBoxes();
    QString strippedName(const QString &fullFileName);
    QString curFile;
    bool isUntitled;
    unsigned int FileType;
};

#endif // NPCEDIT_H
