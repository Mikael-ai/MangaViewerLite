#ifndef MANGAVIEWER_H
#define MANGAVIEWER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MangaViewer; }
QT_END_NAMESPACE

class BaseScrollBar;
class BaseSettings;
class QBoxLayout;
class QLabel;

class MangaViewer : public QMainWindow
{
    Q_OBJECT

private:
    QBoxLayout *layout;
    BaseScrollBar *vScrollBar;
    BaseScrollBar *hScrollBar;

    uint32_t m_sheetWidth;

    QPoint lastContextMenuPos;
    QString lastChosenPath;
    QString currentFilePath;

    QHash<QLabel*, QPixmap> sheets;

public:
    MangaViewer(QWidget *parent = nullptr);
    ~MangaViewer();

    void openFolder(const QString &path);
    void openFiles(QStringList &urls);
    void setWidthValue(const uint32_t newWitdh);

    void updateStyle(const uint32_t sheetWidth,
                     const uint32_t vScrollStep,
                     const uint32_t hScrollStep,
                     const QString &background);

    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void showContextMenu(const QPoint &pos);
    void loadConfigFromVariant(const QVariantMap &config);
    void updateSheetWidth();
    void closeCurrentManga();

    void actionExitClicked();
    void actionSettingsClicked();
    void actionOpenMangaFileClicked();
    void actionOpenFolderClicked();
    void actionOpenImagesClicked();

signals:
    void onWidthValueChanged();

private:
    Ui::MangaViewer *ui;
};
#endif // MANGAVIEWER_H
