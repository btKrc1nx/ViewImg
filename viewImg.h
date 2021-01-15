#include"qmainwindow.h"
#include"ui_viewImg.h"
#include<QMessageBox>
#include<QLabel>
#include<QFileDialog>
#include<QDir>
#include<QFile>
#include<QIODevice>
#include<QMenu>
#include<QPoint>
#include<QMouseEvent>
#include<QResizeEvent>
class Practice:public QMainWindow {
    Q_OBJECT
public:
    Practice(QWidget *parent = nullptr);
    ~Practice();
private:
    Ui::PracticeClass ui;
    /*自定义TreeWIdgetItem的枚举类型必须大于1000,1000以内留给实现*/
    enum treeItemType{itTopItem=1001,itGroupItem,itImageItem};
    enum treeColumnNum{colItem,colItemType};
    void iniUI();
    void iniTree();
    void addFolderItem(QTreeWidgetItem *parentItem, const QString &dirName);
    void addImageItem(QTreeWidgetItem *parentItem, const QString &fileName);
    void recursionTreeItem(QTreeWidgetItem *item);
    void disPlayImage(QTreeWidgetItem *item);
    bool checkTreeWidgetItem(QTreeWidgetItem *item);
    QString getFinalFolderName(const QString &fullPathName) const;
    void redrawPixmap(const QPixmap *pixmap);

    QLabel *labFileName;
    QPixmap currentPixmap;
    QPoint mLastRelPos, newWindowPos;
    bool isDraging;
    double pixRatio;
private slots:
    void onActAddFolderTriggered();
    void onActAddFilesTriggered();
    void onActDeleteItemTriggered();
    void onActTraversalTriggered();
    void onActZoomFitHTriggered();
    void onActZoomFitWTriggered();
    void onActZoomInTriggered();
    void onActZoomOutTriggered();
    void onActZoomRealSizeTriggered();
    void onActDockVisibleToggled(bool arg1);
    void onActDockFloatTriggered(bool checked);
    void onDockWidgetVisibilityChanged(bool visible);
    void onDockTopLevelChanged(bool topLevel);
    void onTreeFilesCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onTreeFilesCustomContextMenuRequired(QPoint pos);

protected:
    void mousePressEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;
};
//int welcome(QWidget *parent=nullptr);