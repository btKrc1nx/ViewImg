#pragma warning(disable:26444)
#include"viewImg.h"

/*手动调用基类构造函数*/
Practice::Practice(QWidget *parent) :QMainWindow(parent) {
    this->isDraging = false;

    ui.setupUi(this);
    iniUI();
    iniTree();
}
Practice::~Practice() {
    /*此label没有父对象，需要手动释放内存
    如果new时指定了this为父亲，则this销毁时会自动执行QLabel的析构函数*/
    delete labFileName;
}
void Practice::iniUI() {
    setCentralWidget(ui.scrollArea);
    setWindowIcon(QIcon(":/pics/vorctorIcon/favicon.ico"));
    setWindowFlags(Qt::FramelessWindowHint);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui.dockWidget->setWidget(ui.treeFiles);
    labFileName = new QLabel("");
    ui.statusbar->addWidget(labFileName);
    ui.toolBar->setMovable(false);
    ui.labDisplayPix->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    ui.labDisplayPix->setIndent(0);
    ui.labDisplayPix->setAlignment(Qt::AlignCenter);
}
void Practice::iniTree() {
    QString dataStr("");//使用直接构造，不使用拷贝构造
    ui.treeFiles->clear();
    QIcon icon;
    icon.addFile(":/images/images/bird.jpg");
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itTopItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, QStringLiteral("图片文件"));
    item->setText(Practice::colItemType, "type=itTopItem");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    item->setData(Practice::colItem, Qt::UserRole, QVariant(dataStr));
    ui.treeFiles->addTopLevelItem(item);
}
/*添加文件夹action槽函数*/
void Practice::onActAddFolderTriggered() {
    if (!checkTreeWidgetItem(ui.treeFiles->currentItem()))return;
    QString directory = QFileDialog::getExistingDirectory(this, QStringLiteral("先有鸿钧后有天，陆压道人还在前"));
    if (!directory.isEmpty()) {
        /*顶层节点和组节点添加group直接以当前项作为父节点*/
        QTreeWidgetItem *parentItem = ui.treeFiles->currentItem();
        /*防止没有选中项时执行添加函数，否则会触发异常*/
        if (!parentItem) {
            QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("没有选中节点"), QMessageBox::Ok);
            return;
        }
        addFolderItem(parentItem, directory);
    }
}
void Practice::addFolderItem(QTreeWidgetItem *parentItem, const QString &dirName) {
    QIcon icon(":/images/images/bird.jpg");
    QString nodeText = getFinalFolderName(dirName);

    /*添加组节点*/
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itGroupItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, nodeText);
    item->setText(Practice::colItemType, "type=itGroupItem");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    item->setData(Practice::colItem, Qt::UserRole, QVariant(dirName));

    parentItem->addChild(item);

    /*添加目录下的文件*/
    QDir dir(dirName);
    QStringList stringFilter("");
    stringFilter << "*.jpg" << "*.png" << "*.webp" << "*.gif";
    /*按名称排序*/
    QStringList names = dir.entryList(stringFilter, QDir::Files | QDir::Readable, QDir::Name);
    for(QString name:names) {
        QTreeWidgetItem *imgItem = new QTreeWidgetItem(Practice::itImageItem);
        imgItem->setIcon(Practice::colItem, icon);
        imgItem->setText(Practice::colItem, name);
        imgItem->setData(Practice::colItem, Qt::UserRole, dirName + "/" + name);
        item->addChild(imgItem);
 
    }

}
QString Practice::getFinalFolderName(const QString &fullPathName)const {
    int count = fullPathName.size();
    int i = fullPathName.lastIndexOf("/");
    return fullPathName.right(count - i - 1);//注意返回引用的注意事项！
}
/*添加文件action槽函数*/
void Practice::onActAddFilesTriggered() {
    /*触发action信号时如果当前项为空则直接返回*/
    if (!checkTreeWidgetItem(ui.treeFiles->currentItem()))return;
    QStringList files = QFileDialog::getOpenFileNames(this, QStringLiteral("选择一个或多个文件"), "", ("Images(*.jpg *.png)"));
    if (files.isEmpty()) return;//stringList为空时直接返回
    QTreeWidgetItem *parentItem, *item;
    item = ui.treeFiles->currentItem();
    /*如果当前节点是图片节点则设置其父节点，顶层节点和group直接以其自身作为父节点*/
    if (item->type() == Practice::itImageItem)
        parentItem = item->parent();
    else
        parentItem = item;
    for (int i = 0; i < files.size(); ++i) {
        addImageItem(parentItem, files[i]);
    }
}
/*遍历所有节点*/
void Practice::onActTraversalTriggered() {
    for (int i = 0; i < ui.treeFiles->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topItem = ui.treeFiles->topLevelItem(i);
        recursionTreeItem(topItem);
    }
}
/*功能函数，对节点遍历*/
void Practice::recursionTreeItem(QTreeWidgetItem *item) {
    QString str = "*" + item->text(Practice::colItem);
    item->setText(Practice::colItem, str);
    if (item->childCount() > 0) {//如果项有子节点则进入递归
        for (int i = 0; i < item->childCount(); ++i)
            recursionTreeItem(item->child(i));
    }
}
/*功能函数，添加图片节点*/
void Practice::addImageItem(QTreeWidgetItem *parentItem, const QString &fileName) {
    QIcon icon(":/images/images/bird.jpg");
    QString nodeText = getFinalFolderName(fileName);
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itImageItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, nodeText);
    item->setText(Practice::colItemType, "type=itImageItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    /*设置完整路径数据*/
    item->setData(Practice::colItem, Qt::UserRole, fileName);
    parentItem->addChild(item);//添加儿子项
}

/*item发生变化时的槽函数*/
void Practice::onTreeFilesCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    /*当前没有选中项时将直接返回*/
    if (!current) {
        return;
    }
    int itemType = current->type();
    switch (itemType) {
    case Practice::itTopItem:
        ui.actAddFiles->setEnabled(true);
        ui.actAddFolder->setEnabled(true);
        ui.actDeleteItem->setEnabled(false);
        break;
    case Practice::itGroupItem:
        ui.actAddFiles->setEnabled(true);
        ui.actAddFolder->setEnabled(true);
        ui.actDeleteItem->setEnabled(true);
        break;
    case Practice::itImageItem:
        ui.actAddFiles->setEnabled(true);
        ui.actAddFolder->setEnabled(false);//图片节点不能添加文件夹
        ui.actDeleteItem->setEnabled(true);
        disPlayImage(current);
        break;
    }
}
/*自定义QTreeWIdget的右键Menu*/
void Practice::onTreeFilesCustomContextMenuRequired(QPoint pos) {
    QMenu *menu = new QMenu();
    menu->addAction(ui.actAddFiles);
    menu->addAction(ui.actAddFolder);
    menu->addAction(ui.actDeleteItem);
    menu->addSeparator();
    menu->addAction(ui.actTraversalItems);
    menu->exec(QCursor::pos());
    delete menu;
}
void Practice::onActDeleteItemTriggered() {
    QTreeWidgetItem *item = ui.treeFiles->currentItem();
    if (!checkTreeWidgetItem(item))return;
    QTreeWidgetItem *parent = item->parent();
    parent->removeChild(item);//从ui移除一个节点，并不会释放内存，类似于list
    delete item;
}
/*适合高度*/
void Practice::onActZoomFitHTriggered() {
    int height = ui.scrollArea->height();
    int realHeight = currentPixmap.height();
    QPixmap pix = currentPixmap.scaledToHeight(height - 30);
    pixRatio = static_cast<double>(height) / realHeight;//保存比例
    ui.labDisplayPix->setPixmap(pix);
}
/*适合宽度*/
void Practice::onActZoomFitWTriggered() {
    int width = ui.scrollArea->width();
    int realWidth = currentPixmap.width();
    QPixmap pix = currentPixmap.scaledToWidth(width);
    pixRatio = static_cast<double>(width) / realWidth;
    ui.labDisplayPix->setPixmap(pix);
}
/*放大图片*/
void Practice::onActZoomInTriggered() {
    pixRatio *= 1.2;//放大1.2倍
    int width = pixRatio * currentPixmap.width();
    int height = pixRatio * currentPixmap.height();
    QPixmap pix = currentPixmap.scaled(width, height);
    ui.labDisplayPix->setPixmap(pix);
}
/*缩小图片*/
void Practice::onActZoomOutTriggered() {
    if (currentPixmap.height() * pixRatio <= ui.scrollArea->height() / 2) {
        QMessageBox::information(this, QStringLiteral("最小了"), QStringLiteral("无法更小"), QMessageBox::Ok);
        return;
    }
    pixRatio /= 1.2;
    int width = pixRatio * currentPixmap.width();
    int height = pixRatio * currentPixmap.height();
    QPixmap pix = currentPixmap.scaled(width, height);
    ui.labDisplayPix->setPixmap(pix);
}
/*显示图片真实大小*/
void Practice::onActZoomRealSizeTriggered() {
    pixRatio = 1;
    ui.labDisplayPix->setPixmap(currentPixmap);
}
void Practice::disPlayImage(QTreeWidgetItem *item) {
    QString fileName = item->data(Practice::colItem, Qt::UserRole).toString();//读取节点信息，转换成字符串
    QFile file(fileName);
    QImage img(fileName);

    bool isOk = file.open(QIODevice::ReadOnly);
    if (isOk) {
        /*不要直接载入文件，QPixmap在处理24位深的图片时会出错*/
        QByteArray data = file.readAll();
        currentPixmap.loadFromData(data);
        redrawPixmap(&currentPixmap);
    }

}

/*功能函数 重新调整图片大小*/
void Practice::redrawPixmap(const QPixmap *pixmap) {
    if(pixmap->height()>pixmap->width())
        this->onActZoomFitHTriggered();
    else
        this->onActZoomFitWTriggered();
}

void Practice::onActDockVisibleToggled(bool arg1) {
    ui.dockWidget->setVisible(arg1);
}
void Practice::onDockWidgetVisibilityChanged(bool visible) {
    ui.actDockVisible->setChecked(visible);
}
void Practice::onActDockFloatTriggered(bool checked) {
    ui.dockWidget->setFloating(checked);
}
void Practice::onDockTopLevelChanged(bool topLevel) {
    ui.actDockFloating->setChecked(topLevel);
}
/*检查当前QTreeWidget是否选中项*/
bool Practice::checkTreeWidgetItem(QTreeWidgetItem *item) {
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("警告"),
            QStringLiteral("没有选中任何项（只有在顶层节点和Group节点可以添加文件夹子节点，\
Image节点添加子节点默认为其父节点，顶层节点无法删除）"), QMessageBox::Ok);
        return false;
    }
    return true;
}

/*使用鼠标拖动窗口*/

void Practice::mousePressEvent(QMouseEvent *event) {
    this->isDraging = true;
    /*得到相对于widget的坐标*/
    this->mLastRelPos = event->globalPos() - this->pos();
    QMainWindow::mousePressEvent(event);

}

void Practice::mouseMoveEvent(QMouseEvent *event) {
    this->newWindowPos = event->globalPos() - this->mLastRelPos;
    if (this->isDraging && (event->buttons() & Qt::LeftButton) &&
        (this->newWindowPos - this->pos()).manhattanLength() > 1) {
        this->move(this->newWindowPos);
        /*保存新的相对坐标*/
        this->mLastRelPos = event->globalPos() - this->pos();
    }
    
    QMainWindow::mouseMoveEvent(event);
}



void Practice::mouseReleaseEvent(QMouseEvent *event) {
    this->isDraging = false;
}


void Practice::resizeEvent(QResizeEvent *event) {
    redrawPixmap(&(this->currentPixmap));
}
