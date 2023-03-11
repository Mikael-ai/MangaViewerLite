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
    void closeCurrentManga();
    void setWidthValue(const uint32_t newWitdh);

    void updateStyle(const uint32_t sheetWidth,
                     const uint32_t vScrollStep,
                     const uint32_t hScrollStep,
                     const QString &background);

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
    void showContextMenu(const QPoint &pos);
    void loadConfigFromVariant(const QVariantMap &config);
    void updateSheetWidth();

    void actionExitClicked();
    void actionSettingsClicked();
    void actionOpenFileClicked();
    void actionOpenFolderClicked();

signals:
    void onWidthValueChanged();

private:
    Ui::MangaViewer *ui;
};
#endif // MANGAVIEWER_H
