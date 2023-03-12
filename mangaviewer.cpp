#include "mangaviewer.h"
#include "ui_mangaviewer.h"
#include "basescrollbar.h"
#include "basesettings.h"
#include "appUtils.h"

#include <QLabel>
#include <QDir>
#include <QMenu>
#include <QApplication>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMimeData>
#include <QImageReader>

MangaViewer::MangaViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MangaViewer)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("Manga Viewer Lite"));
    this->setAcceptDrops(true);

    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setAlignment(Qt::AlignCenter);

    vScrollBar = new BaseScrollBar(Qt::Vertical, ui->scrollArea);
    hScrollBar = new BaseScrollBar(Qt::Horizontal, ui->scrollArea);

    ui->scrollArea->setVerticalScrollBar(vScrollBar);
    ui->scrollArea->setHorizontalScrollBar(hScrollBar);

    ui->centralwidget->setContextMenuPolicy(Qt::CustomContextMenu);

    const QString background = appUtils->getConfigValue(key_background).toString();
    ui->scrollArea->setStyleSheet(appUtils->getBigAssScrollAreaStyleSheet(background));

    loadConfigFromVariant(appUtils->getConfig(true));

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

    if (path != appUtils->getTempDirPath())
        currentFilePath = path;
}

void MangaViewer::openFiles(QStringList &urls)
{
    closeCurrentManga();
    urls.sort(Qt::CaseInsensitive);

    foreach (QString url, urls)
    {
        if (QImageReader(url).imageFormat() == QImage::Format_Invalid)
            continue;

        if (url.startsWith("file:///"))
            url.remove("file:///");

        QPixmap sheetPixmap(url);
        QLabel *sheet = new QLabel(ui->scrollAreaWidgetContents);
        sheet->setPixmap((m_sheetWidth != 0)
                         ? sheetPixmap.scaledToWidth(m_sheetWidth,
                                                     Qt::SmoothTransformation)
                         : sheetPixmap);
        layout->addWidget(sheet);
        sheet->setAlignment(Qt::AlignHCenter);
        sheets[sheet] = sheetPixmap;
    }
}

void MangaViewer::closeCurrentManga()
{
    if (sheets.isEmpty())
        return;

    foreach (QLabel *sheet, sheets.keys())
    {
        delete sheet;
        sheet = Q_NULLPTR;
    }
    sheets.clear();

    appUtils->cleanTempDir();
}

void MangaViewer::manageBorder()
{
    ui->scrollArea->setFrameShape((ui->scrollArea->frameShape() != QFrame::NoFrame)
                                  ? QFrame::NoFrame
                                  : QFrame::StyledPanel);
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
    const uint32_t sheetWidth = config.value(key_sheetWidth, defaultSheetWidth).toUInt();
    const uint32_t vScrollStep = config.value(key_vScrollStep, defaultVScrollStep).toUInt();
    const uint32_t hScrollStep = config.value(key_hScrollStep, defaultHScrollStep).toUInt();
    const QString background = config.value(key_background, defaultBackground).toString();

    updateStyle(sheetWidth,
                vScrollStep,
                hScrollStep,
                background);
}

void MangaViewer::updateStyle(const uint32_t sheetWidth,
                              const uint32_t vScrollStep,
                              const uint32_t hScrollStep,
                              const QString &background)
{
    setWidthValue(sheetWidth);
    vScrollBar->setSingleStep(vScrollStep);
    hScrollBar->setSingleStep(hScrollStep);
    ui->centralwidget->setStyleSheet(appUtils->constructStyleSheet("centalWidget",
                                                                   background));
    ui->scrollArea->setStyleSheet(appUtils->getBigAssScrollAreaStyleSheet(background));
}

void MangaViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->accept();
    else
        event->ignore();
}

void MangaViewer::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MangaViewer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        event->ignore();

    event->accept();
    if (urls.size() == 1)
    {
        QFileInfo file(event->mimeData()->text().remove("file:///"));
        if (file.isDir())
        {
            openFolder(file.absoluteFilePath());
            return;
        }
        else if (file.isFile())
        {
            const QString extention = file.suffix();
            if (extention == QStringLiteral("cbr")
                || extention == QStringLiteral("zip")
                || extention == QStringLiteral("rar")
                || extention == QStringLiteral("7z"))
            {
                appUtils->cleanTempDir();
                appUtils->unzipFile(file.absoluteFilePath());
                openFolder(appUtils->getTempDirPath());
                return;
            }
        }
    }

    QStringList urlsList = QUrl::toStringList(urls);
    openFiles(urlsList);

    event->accept();
}

bool MangaViewer::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Space
            || ke->key() == Qt::Key_F11)
        {
            setWindowState((windowState() == Qt::WindowFullScreen)
                           ? Qt::WindowNoState
                           : Qt::WindowFullScreen);
            return true;
        }
    }
    return QMainWindow::event(event);
}

void MangaViewer::closeEvent(QCloseEvent *event)
{
    appUtils->clean();
    return QMainWindow::closeEvent(event);
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
    // Sub menu
    QScopedPointer<QMenu> openFileSubMenu(contextMenu->addMenu("Open..."));
    openFileSubMenu->addAction(tr("Manga file"), this, SLOT(actionOpenMangaFileClicked()));
    openFileSubMenu->addAction(tr("Folder"), this, SLOT(actionOpenFolderClicked()));
    openFileSubMenu->addAction(tr("Images"), this, SLOT(actionOpenImagesClicked()));
    // Main menu
    contextMenu->addAction(tr("Close file"), this, SLOT(closeCurrentManga()));

    // Oof
    contextMenu->addAction(tr((ui->scrollArea->frameShape() != QFrame::NoFrame)
                              ? "Disable border"
                              : "Enable border"),
                           this,
                           SLOT(manageBorder()));

    contextMenu->addAction(tr("Settings"), this, SLOT(actionSettingsClicked()));
    contextMenu->addAction(tr("Exit"), this, SLOT(actionExitClicked()));

    const QFont sitkaText("Sitka Text", 14);
    const QString styleSheet = appUtils->getStyleSheet("menu");

    contextMenu->setFont(sitkaText);
    contextMenu->setStyleSheet(styleSheet);
    contextMenu->setMinimumSize(QSize(100, 70));
    contextMenu->setAttribute(Qt::WA_TranslucentBackground);
    contextMenu->setWindowFlag(Qt::FramelessWindowHint);

    openFileSubMenu->setFont(sitkaText);
    openFileSubMenu->setAttribute(Qt::WA_TranslucentBackground);
    contextMenu->setStyleSheet(styleSheet);
    openFileSubMenu->setWindowFlag(Qt::FramelessWindowHint);

    QPoint globalPosition = this->mapToGlobal(pos);
    contextMenu->exec(globalPosition);
}

void MangaViewer::actionOpenMangaFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"),
                                                    QDir(lastChosenPath).exists()
                                                    ? lastChosenPath
                                                    : lastChosenPath = QDir::currentPath(),
                                                    tr("Manga (*.cbr *.zip *.rar *.7z)"));
    if (filePath.isEmpty())
        return;

    appUtils->unzipFile(filePath);
    openFolder(appUtils->getTempDirPath());
}

void MangaViewer::actionOpenFolderClicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                           tr("Open file"),
                                                           QDir(lastChosenPath).exists()
                                                           ? lastChosenPath
                                                           : lastChosenPath = QDir::currentPath(),
                                                           QFileDialog::ShowDirsOnly);
    if (folderPath.isEmpty())
        return;

    openFolder(folderPath);
}

void MangaViewer::actionOpenImagesClicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir(lastChosenPath).exists()
                        ? lastChosenPath
                        : lastChosenPath = QDir::currentPath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.jpeg *.xpm *.bmp *.gif)"));

    QStringList images;
    if (dialog.exec())
        images = dialog.selectedFiles();

    openFiles(images);
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

