#ifndef ANCHORMANAGER_H
#define ANCHORMANAGER_H

#include <QObject>
#include <QWidget>
#include <QMargins>

class AnchorManagerPrivate;
class AnchorManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AnchorManager)
public:
    enum AnchorsLine {
        Left,
        Top,
        Right,
        Bottom,
        HorizontalCenter,
        VerticalCenter,
    };

    explicit AnchorManager(QObject *parent = Q_NULLPTR);
    ~AnchorManager();

    void anchorFill(QWidget *targetWidget, QWidget *parentWidget);
    void centerIn(QWidget *targetWidget, QWidget *parentWidget);
    void anchor(QWidget *targetWidget, AnchorsLine targetAnchorLine, QWidget *anchorWidget, AnchorsLine anchorLine);

    void setMargins(QWidget *targetWidget, const QMargins &margins);
    QMargins margins(QWidget *targetWidget) const;

    void setTopMargin(QWidget *targetWidget, int margin);
    int topMargin(QWidget *targetWidget) const;

    void setBottomMargin(QWidget *targetWidget, int margin);
    int bottomMargin(QWidget *targetWidget) const;

    void setLeftMargin(QWidget *targetWidget, int margin);
    int leftMargin(QWidget *targetWidget) const;

    void setRightMargin(QWidget *targetWidget, int margin);
    int rightMargin(QWidget *targetWidget) const;

    void update(QWidget *targetWidget);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

protected:
    QScopedPointer<AnchorManagerPrivate> d_ptr;
};

#endif // ANCHORMANAGER_H
