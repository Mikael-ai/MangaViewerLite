#include "mangaviewer.h"
#include "ui_mangaviewer.h"
#include "basescrollbar.h"
#include "basesettings.h"
#include "projectGlobalSettings.h"

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
    scrollBar = new BaseScrollBar(ui->scrollArea);
    lastContextMenuPos = QPoint();

    layout->setAlignment(Qt::AlignCenter);

    ui->scrollArea->setVerticalScrollBar(scrollBar);
    ui->centralwidget->setContextMenuPolicy(Qt::CustomContextMenu);

    settingsWidget = new BaseSettings();

    loadConfigFromVariant(settingsWidget->getConfig(false));

    openFolder("C:/Users/Mikael/Documents/Manga/Claymore/1-1");

    connect(ui->centralwidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(settingsWidget, SIGNAL(settingsWereSaved(QVariantMap)),
            this, SLOT(loadConfigFromVariant(QVariantMap)));
    connect(this, &MangaViewer::onWidthValueChanged,
            this, &MangaViewer::updateSheetWidth);
}

MangaViewer::~MangaViewer()
{
    if (scrollBar)
        delete scrollBar;
    if (settingsWidget)
        delete settingsWidget;

    delete ui;
}

void MangaViewer::openFolder(const QString &path)
{
    QDir dir(path);
    if (dir.isEmpty())
        return;

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

void MangaViewer::setWidthValue(const uint32_t newWitdh)
{
    if (m_sheetWidth == newWitdh)
        return;

    m_sheetWidth = newWitdh;
    emit onWidthValueChanged();
}

void MangaViewer::loadConfigFromVariant(const QVariantMap &config)
{
    setWidthValue(config.value("sheetWidth", defaultSheetWidth).toUInt());
    m_scrollStep = config.value("scrollStep", defaultScrollStep).toUInt();
    m_background = config.value("background", defaultBackground).toString();

    scrollBar->setSingleStep(m_scrollStep);
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
                                                    tr("Manga Files (*.zip *.cbr *.rar *.7z)"));
}

void MangaViewer::actionOpenFolderClicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this,
                                                         tr("Open file"),
                                                         QDir(lastChosenPath).exists()
                                                         ? lastChosenPath
                                                         : lastChosenPath = QDir::currentPath(),
                                                         QFileDialog::ShowDirsOnly);
}

void MangaViewer::actionSettingsClicked()
{
    settingsWidget->setUiValues();

    QPoint globalPosition = this->mapToGlobal(lastContextMenuPos);
    settingsWidget->move(globalPosition);
    settingsWidget->show();
}

void MangaViewer::actionExitClicked()
{
    QApplication::quit();
}

