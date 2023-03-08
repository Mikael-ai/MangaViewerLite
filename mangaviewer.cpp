#include "mangaviewer.h"
#include "ui_mangaviewer.h"
#include "basescrollbar.h"
#include "basesettings.h"
#include "appUtils.h"

#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QMenu>
#include <QApplication>
#include <QJsonDocument>
#include <QFileDialog>

MangaViewer::MangaViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MangaViewer)
{
    ui->setupUi(this);

    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setAlignment(Qt::AlignCenter);

    vScrollBar = new BaseScrollBar(Qt::Vertical, ui->scrollArea);
    hScrollBar = new BaseScrollBar(Qt::Horizontal, ui->scrollArea);

    ui->scrollArea->setVerticalScrollBar(vScrollBar);
    ui->scrollArea->setHorizontalScrollBar(hScrollBar);
    ui->centralwidget->setContextMenuPolicy(Qt::CustomContextMenu);

    loadConfigFromVariant(appUtils->getConfig(true));

    //openFolder("C:/Users/Mikael/Documents/Manga/Claymore/1-1");

    connect(ui->centralwidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    connect(this, &MangaViewer::onWidthValueChanged,
            this, &MangaViewer::updateSheetWidth);
}

MangaViewer::~MangaViewer()
{
    if (vScrollBar)
        delete vScrollBar;

    delete ui;
}

void MangaViewer::openFolder(const QString &path)
{
    QDir dir(path);
    if (dir.isEmpty())
        return;

    closeCurrentManga();

    QFileInfoList list  = dir.entryInfoList();
    foreach (const QFileInfo &fileInfo, list)
    {
        if (!fileInfo.isFile())
            continue;

        QPixmap sheetPixmap(fileInfo.absoluteFilePath());
        QLabel *sheet = new QLabel(ui->scrollAreaWidgetContents);
        sheet->setPixmap((m_sheetWidth != 0)
                         ? sheetPixmap.scaledToWidth(m_sheetWidth,
                                                     Qt::SmoothTransformation)
                         : sheetPixmap);
        layout->addWidget(sheet);
        sheet->setAlignment(Qt::AlignHCenter);
        sheets[sheet] = sheetPixmap;
    }
    currentFilePath = path;
}

void MangaViewer::closeCurrentManga()
{
    if (sheets.isEmpty())
        return;

    QLayoutItem *child = nullptr;
    while ((child = layout->takeAt(0)))
    {
        delete child;
        child = Q_NULLPTR;
    }
    sheets.clear();
}

void MangaViewer::setWidthValue(const uint32_t newWitdh)
{
    if (m_sheetWidth == newWitdh)
        return;

    m_sheetWidth = newWitdh;
    emit onWidthValueChanged();
}

void MangaViewer::loadConfigFromVariant(const QVariantMap &config)
{
    setWidthValue(config.value(key_sheetWidth, defaultSheetWidth).toUInt());
    vScrollBar->setSingleStep(config.value(key_vScrollStep, defaultVScrollStep).toUInt());
    hScrollBar->setSingleStep(config.value(key_hScrollStep, defaultHScrollStep).toUInt());
    m_background = config.value(key_background, defaultBackground).toString();
}

void MangaViewer::updateSheetWidth()
{
    foreach (auto &sheet, sheets.keys())
    {
        sheet->setPixmap((m_sheetWidth != 0)
                         ? sheets[sheet].scaledToWidth(m_sheetWidth,
                                                       Qt::SmoothTransformation)
                         : sheets[sheet]);
    }
}

void MangaViewer::showContextMenu(const QPoint &pos)
{
    lastContextMenuPos = pos;

    QScopedPointer<QMenu> contextMenu(new QMenu());
    QScopedPointer<QMenu> openFileSubMenu(contextMenu->addMenu("Open..."));
    openFileSubMenu->addAction(tr("File"), this, SLOT(actionOpenFileClicked()));
    openFileSubMenu->addAction(tr("Folder"), this, SLOT(actionOpenFolderClicked()));
    contextMenu->addAction(tr("Settings"), this, SLOT(actionSettingsClicked()));
    contextMenu->addAction(tr("Exit"), this, SLOT(actionExitClicked()));

    QPoint globalPosition = this->mapToGlobal(pos);
    contextMenu->exec(globalPosition);
}

void MangaViewer::actionOpenFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"),
                                                    QDir(lastChosenPath).exists()
                                                    ? lastChosenPath
                                                    : lastChosenPath = QDir::currentPath(),
                                                    tr("Manga (*.cbr *.zip)"));
}

void MangaViewer::actionOpenFolderClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                           tr("Open file"),
                                                           QDir(lastChosenPath).exists()
                                                           ? lastChosenPath
                                                           : lastChosenPath = QDir::currentPath(),
                                                           QFileDialog::ShowDirsOnly);
    openFolder(folderPath);
}

void MangaViewer::actionSettingsClicked()
{
    QScopedPointer<BaseSettings, ScopedPointerCustomDeleter> settingsWidget(new BaseSettings(this));
    settingsWidget->setWindowModality(Qt::ApplicationModal);

    QPoint globalPosition = this->mapToGlobal(lastContextMenuPos);
    settingsWidget->move(globalPosition);
    settingsWidget->show();
}

void MangaViewer::actionExitClicked()
{
    QApplication::quit();
}

