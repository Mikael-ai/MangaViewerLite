#ifndef MANGAVIEWER_H
#define MANGAVIEWER_H

#include <QMainWindow>
#include <QBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MangaViewer; }
QT_END_NAMESPACE

class BaseScrollBar;
class BaseSettings;
class QLabel;

class MangaViewer : public QMainWindow
{
    Q_OBJECT

private:
    QBoxLayout *layout;
    BaseScrollBar *scrollBar;
    BaseSettings *settingsWidget;

    uint32_t m_sheetWidth;
    uint32_t m_scrollStep;
    QString m_background;

    QPoint lastContextMenuPos;
    QString lastChosenPath;
    QString currentFilePath;

    QHash<QLabel*, QPixmap> sheets;

public:
    MangaViewer(QWidget *parent = nullptr);
    ~MangaViewer();

    void openFolder(const QString &path);
    void setWidthValue(const uint32_t newWitdh);

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
