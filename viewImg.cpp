#pragma warning(disable:26444)
#include"viewImg.h"

/*�ֶ����û��๹�캯��*/
Practice::Practice(QWidget *parent) :QMainWindow(parent) {
    this->isDraging = false;

    ui.setupUi(this);
    iniUI();
    iniTree();
}
Practice::~Practice() {
    /*��labelû�и�������Ҫ�ֶ��ͷ��ڴ�
    ���newʱָ����thisΪ���ף���this����ʱ���Զ�ִ��QLabel����������*/
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
    QString dataStr("");//ʹ��ֱ�ӹ��죬��ʹ�ÿ�������
    ui.treeFiles->clear();
    QIcon icon;
    icon.addFile(":/images/images/bird.jpg");
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itTopItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, QStringLiteral("ͼƬ�ļ�"));
    item->setText(Practice::colItemType, "type=itTopItem");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    item->setData(Practice::colItem, Qt::UserRole, QVariant(dataStr));
    ui.treeFiles->addTopLevelItem(item);
}
/*����ļ���action�ۺ���*/
void Practice::onActAddFolderTriggered() {
    if (!checkTreeWidgetItem(ui.treeFiles->currentItem()))return;
    QString directory = QFileDialog::getExistingDirectory(this, QStringLiteral("���к�������죬½ѹ���˻���ǰ"));
    if (!directory.isEmpty()) {
        /*����ڵ����ڵ����groupֱ���Ե�ǰ����Ϊ���ڵ�*/
        QTreeWidgetItem *parentItem = ui.treeFiles->currentItem();
        /*��ֹû��ѡ����ʱִ����Ӻ���������ᴥ���쳣*/
        if (!parentItem) {
            QMessageBox::warning(this, QStringLiteral("����"), QStringLiteral("û��ѡ�нڵ�"), QMessageBox::Ok);
            return;
        }
        addFolderItem(parentItem, directory);
    }
}
void Practice::addFolderItem(QTreeWidgetItem *parentItem, const QString &dirName) {
    QIcon icon(":/images/images/bird.jpg");
    QString nodeText = getFinalFolderName(dirName);

    /*�����ڵ�*/
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itGroupItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, nodeText);
    item->setText(Practice::colItemType, "type=itGroupItem");
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    item->setData(Practice::colItem, Qt::UserRole, QVariant(dirName));

    parentItem->addChild(item);

    /*���Ŀ¼�µ��ļ�*/
    QDir dir(dirName);
    QStringList stringFilter("");
    stringFilter << "*.jpg" << "*.png" << "*.webp" << "*.gif";
    /*����������*/
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
    return fullPathName.right(count - i - 1);//ע�ⷵ�����õ�ע�����
}
/*����ļ�action�ۺ���*/
void Practice::onActAddFilesTriggered() {
    /*����action�ź�ʱ�����ǰ��Ϊ����ֱ�ӷ���*/
    if (!checkTreeWidgetItem(ui.treeFiles->currentItem()))return;
    QStringList files = QFileDialog::getOpenFileNames(this, QStringLiteral("ѡ��һ�������ļ�"), "", ("Images(*.jpg *.png)"));
    if (files.isEmpty()) return;//stringListΪ��ʱֱ�ӷ���
    QTreeWidgetItem *parentItem, *item;
    item = ui.treeFiles->currentItem();
    /*�����ǰ�ڵ���ͼƬ�ڵ��������丸�ڵ㣬����ڵ��groupֱ������������Ϊ���ڵ�*/
    if (item->type() == Practice::itImageItem)
        parentItem = item->parent();
    else
        parentItem = item;
    for (int i = 0; i < files.size(); ++i) {
        addImageItem(parentItem, files[i]);
    }
}
/*�������нڵ�*/
void Practice::onActTraversalTriggered() {
    for (int i = 0; i < ui.treeFiles->topLevelItemCount(); ++i) {
        QTreeWidgetItem *topItem = ui.treeFiles->topLevelItem(i);
        recursionTreeItem(topItem);
    }
}
/*���ܺ������Խڵ����*/
void Practice::recursionTreeItem(QTreeWidgetItem *item) {
    QString str = "*" + item->text(Practice::colItem);
    item->setText(Practice::colItem, str);
    if (item->childCount() > 0) {//��������ӽڵ������ݹ�
        for (int i = 0; i < item->childCount(); ++i)
            recursionTreeItem(item->child(i));
    }
}
/*���ܺ��������ͼƬ�ڵ�*/
void Practice::addImageItem(QTreeWidgetItem *parentItem, const QString &fileName) {
    QIcon icon(":/images/images/bird.jpg");
    QString nodeText = getFinalFolderName(fileName);
    QTreeWidgetItem *item = new QTreeWidgetItem(Practice::itImageItem);
    item->setIcon(Practice::colItem, icon);
    item->setText(Practice::colItem, nodeText);
    item->setText(Practice::colItemType, "type=itImageItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsAutoTristate);
    item->setCheckState(Practice::colItem, Qt::Checked);
    /*��������·������*/
    item->setData(Practice::colItem, Qt::UserRole, fileName);
    parentItem->addChild(item);//��Ӷ�����
}

/*item�����仯ʱ�Ĳۺ���*/
void Practice::onTreeFilesCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    /*��ǰû��ѡ����ʱ��ֱ�ӷ���*/
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
        ui.actAddFolder->setEnabled(false);//ͼƬ�ڵ㲻������ļ���
        ui.actDeleteItem->setEnabled(true);
        disPlayImage(current);
        break;
    }
}
/*�Զ���QTreeWIdget���Ҽ�Menu*/
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
    parent->removeChild(item);//��ui�Ƴ�һ���ڵ㣬�������ͷ��ڴ棬������list
    delete item;
}
/*�ʺϸ߶�*/
void Practice::onActZoomFitHTriggered() {
    int height = ui.scrollArea->height();
    int realHeight = currentPixmap.height();
    QPixmap pix = currentPixmap.scaledToHeight(height - 30);
    pixRatio = static_cast<double>(height) / realHeight;//�������
    ui.labDisplayPix->setPixmap(pix);
}
/*�ʺϿ��*/
void Practice::onActZoomFitWTriggered() {
    int width = ui.scrollArea->width();
    int realWidth = currentPixmap.width();
    QPixmap pix = currentPixmap.scaledToWidth(width);
    pixRatio = static_cast<double>(width) / realWidth;
    ui.labDisplayPix->setPixmap(pix);
}
/*�Ŵ�ͼƬ*/
void Practice::onActZoomInTriggered() {
    pixRatio *= 1.2;//�Ŵ�1.2��
    int width = pixRatio * currentPixmap.width();
    int height = pixRatio * currentPixmap.height();
    QPixmap pix = currentPixmap.scaled(width, height);
    ui.labDisplayPix->setPixmap(pix);
}
/*��СͼƬ*/
void Practice::onActZoomOutTriggered() {
    if (currentPixmap.height() * pixRatio <= ui.scrollArea->height() / 2) {
        QMessageBox::information(this, QStringLiteral("��С��"), QStringLiteral("�޷���С"), QMessageBox::Ok);
        return;
    }
    pixRatio /= 1.2;
    int width = pixRatio * currentPixmap.width();
    int height = pixRatio * currentPixmap.height();
    QPixmap pix = currentPixmap.scaled(width, height);
    ui.labDisplayPix->setPixmap(pix);
}
/*��ʾͼƬ��ʵ��С*/
void Practice::onActZoomRealSizeTriggered() {
    pixRatio = 1;
    ui.labDisplayPix->setPixmap(currentPixmap);
}
void Practice::disPlayImage(QTreeWidgetItem *item) {
    QString fileName = item->data(Practice::colItem, Qt::UserRole).toString();//��ȡ�ڵ���Ϣ��ת�����ַ���
    QFile file(fileName);
    QImage img(fileName);

    bool isOk = file.open(QIODevice::ReadOnly);
    if (isOk) {
        /*��Ҫֱ�������ļ���QPixmap�ڴ���24λ���ͼƬʱ�����*/
        QByteArray data = file.readAll();
        currentPixmap.loadFromData(data);
        redrawPixmap(&currentPixmap);
    }

}

/*���ܺ��� ���µ���ͼƬ��С*/
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
/*��鵱ǰQTreeWidget�Ƿ�ѡ����*/
bool Practice::checkTreeWidgetItem(QTreeWidgetItem *item) {
    if (!item) {
        QMessageBox::warning(this, QStringLiteral("����"),
            QStringLiteral("û��ѡ���κ��ֻ���ڶ���ڵ��Group�ڵ��������ļ����ӽڵ㣬\
Image�ڵ�����ӽڵ�Ĭ��Ϊ�丸�ڵ㣬����ڵ��޷�ɾ����"), QMessageBox::Ok);
        return false;
    }
    return true;
}

/*ʹ������϶�����*/

void Practice::mousePressEvent(QMouseEvent *event) {
    this->isDraging = true;
    /*�õ������widget������*/
    this->mLastRelPos = event->globalPos() - this->pos();
    QMainWindow::mousePressEvent(event);

}

void Practice::mouseMoveEvent(QMouseEvent *event) {
    this->newWindowPos = event->globalPos() - this->mLastRelPos;
    if (this->isDraging && (event->buttons() & Qt::LeftButton) &&
        (this->newWindowPos - this->pos()).manhattanLength() > 1) {
        this->move(this->newWindowPos);
        /*�����µ��������*/
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
