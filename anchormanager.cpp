#include "anchormanager.h"
#include <QDebug>
#include <QEvent>

struct AnchorData
{
    // 锚定在窗口的哪个位置上
    QHash<AnchorManager::AnchorsLine, QPair<QWidget *, AnchorManager::AnchorsLine> > mAnchorsLine;
    QMargins mMargins;
};

class AnchorManagerPrivate
{
public:
    QHash<QWidget *, AnchorData> mItems;                          // 存储锚布局的数据
    QHash<QWidget *, QList<QWidget *> > mAnchorItems;  // 存储反向序列以便查询

    void updateWidget(QWidget *targetWidget);
};

void AnchorManagerPrivate::updateWidget(QWidget *targetWidget)
{
    AnchorData data = mItems.value(targetWidget);

    // 把边框考虑进去
    int x = targetWidget->x();
    int y = targetWidget->y();
    int w = targetWidget->width();
    int h = targetWidget->height();

    if (data.mAnchorsLine.contains(AnchorManager::Left)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::Left);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

        if (value.second == AnchorManager::Left)
            x = isParent ? 0 : anchorRect.x();
        else if (value.second == AnchorManager::HorizontalCenter)
            x = isParent ? value.first->width() / 2 : anchorRect.center().x();
        else if (value.second == AnchorManager::Right)
            x = isParent ? value.first->width() : anchorRect.x() + anchorRect.width();

        x += data.mMargins.left();
    }

    if (data.mAnchorsLine.contains(AnchorManager::Top)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::Top);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

        if (value.second == AnchorManager::Top)
            y = isParent ? 0 : anchorRect.y();
        else if (value.second == AnchorManager::VerticalCenter)
            y = isParent ? value.first->height() / 2 : anchorRect.center().y();
        else if (value.second == AnchorManager::Bottom)
            y = isParent ? value.first->height() : anchorRect.y() + anchorRect.height();

        y += data.mMargins.top();
    }

    if (data.mAnchorsLine.contains(AnchorManager::Right)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::Right);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

        int anchorX = 0;
        if (value.second == AnchorManager::Left)
            anchorX = isParent ? 0 : anchorRect.x();
        else if (value.second == AnchorManager::HorizontalCenter)
            anchorX = isParent ? value.first->width() / 2 : anchorRect.center().x();
        else if (value.second == AnchorManager::Right)
            anchorX = isParent ? value.first->width() : anchorRect.x() + anchorRect.width();

        // 这里要把边框的宽度考虑进去
        int frameWidth = targetWidget->frameGeometry().width() - w;
        int right = anchorX - data.mMargins.right() - frameWidth;

        if (data.mAnchorsLine.contains(AnchorManager::Left)) { // 已有锚定，改变宽度
            w = qMax(right - x, 0);
            w = (x + w + data.mMargins.right() + frameWidth > anchorX) ? 0 : w;
        } else {  // 没有锚布局，改变位置
            x = right - w;
        }
    }

    if (data.mAnchorsLine.contains(AnchorManager::Bottom)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::Bottom);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

        int anchorY = 0;
        if (value.second == AnchorManager::Top)
            anchorY = isParent ? 0 : anchorRect.y();
        else if (value.second == AnchorManager::VerticalCenter)
            anchorY = isParent ? value.first->height() / 2 : anchorRect.center().y();
        else if (value.second == AnchorManager::Bottom)
            anchorY = isParent ? value.first->height() : anchorRect.y() + anchorRect.height();

        // 这里要把边框高度考虑进去
        int frameHeight = targetWidget->frameGeometry().height() - h;
        int bottom = anchorY - data.mMargins.bottom() - frameHeight;

        if (data.mAnchorsLine.contains(AnchorManager::Top)) {  // 已有锚定，改变高度
            h = qMax(bottom - y, 0);
            h = (y + h + data.mMargins.bottom() + frameHeight > anchorY) ? 0 : h;
        } else {   // 没有锚定，改变位置
            y = bottom - h;
        }
    }

    if (data.mAnchorsLine.contains(AnchorManager::HorizontalCenter)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::HorizontalCenter);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

        int anchorX = 0;
        if (value.second == AnchorManager::Left)
            anchorX = isParent ? 0 : anchorRect.x();
        else if (value.second == AnchorManager::HorizontalCenter)
            anchorX = isParent ? value.first->width() / 2 : anchorRect.center().x();
        else if (value.second == AnchorManager::Right)
            anchorX = isParent ? value.first->width() : anchorRect.x() + anchorRect.width();

        x = anchorX - targetWidget->frameGeometry().width() / 2;
    }

    if (data.mAnchorsLine.contains(AnchorManager::VerticalCenter)) {
        QPair<QWidget *, AnchorManager::AnchorsLine> value = data.mAnchorsLine.value(AnchorManager::VerticalCenter);
        QRect anchorRect = value.first->frameGeometry();

        bool isParent = (targetWidget->parentWidget() == value.first);

       int anchorY = 0;
       if (value.second == AnchorManager::Top)
           anchorY = isParent ? 0 : anchorRect.y();
       else if (value.second == AnchorManager::VerticalCenter)
           anchorY = isParent ? value.first->height() / 2 : anchorRect.center().y();
       else if (value.second == AnchorManager::Bottom)
           anchorY = isParent ? value.first->height() : anchorRect.y() + anchorRect.height();

       y = anchorY - targetWidget->frameGeometry().height() / 2;
    }


    targetWidget->move(x, y);
    targetWidget->resize(w, h);
}

AnchorManager::AnchorManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new AnchorManagerPrivate)
{

}

AnchorManager::~AnchorManager()
{

}

void AnchorManager::anchorFill(QWidget *targetWidget, QWidget *parentWidget)
{
    if (targetWidget == Q_NULLPTR || parentWidget == Q_NULLPTR) {
        qWarning() << Q_FUNC_INFO << "Can't anchor: targetWidget(" << targetWidget
                   << "), parent(" << parentWidget << ")";
        return;
    }

    Q_D(AnchorManager);
    targetWidget->setParent(parentWidget); // 重新设置父窗口

    AnchorData data = d->mItems.value(targetWidget);
    data.mAnchorsLine.clear();
    d->mItems[targetWidget] = data;

    anchor(targetWidget, Top, parentWidget, Top);
    anchor(targetWidget, Bottom, parentWidget, Bottom);
    anchor(targetWidget, Left, parentWidget, Left);
    anchor(targetWidget, Right, parentWidget, Right);
}

void AnchorManager::centerIn(QWidget *targetWidget, QWidget *parentWidget)
{
    if (targetWidget == Q_NULLPTR || parentWidget == Q_NULLPTR) {
        qWarning() << Q_FUNC_INFO << "Can't anchor: targetWidget(" << targetWidget
                   << "), parent(" << parentWidget << ")";
        return;
    }

    Q_D(AnchorManager);
    targetWidget->setParent(parentWidget); // 重新设置父窗口

    AnchorData data = d->mItems.value(targetWidget);
    data.mAnchorsLine.clear();
    d->mItems[targetWidget] = data;

    anchor(targetWidget, HorizontalCenter, parentWidget, HorizontalCenter);
    anchor(targetWidget, VerticalCenter, parentWidget, VerticalCenter);
}

void AnchorManager::anchor(QWidget *targetWidget, AnchorManager::AnchorsLine targetAnchorLine,
                           QWidget *anchorWidget, AnchorManager::AnchorsLine anchorLine)
{
    if (targetWidget == Q_NULLPTR || anchorWidget == Q_NULLPTR) {
        qWarning() << Q_FUNC_INFO << "Can't anchor: targetWidget(" << targetWidget
                   << "), anchorWidget(" << anchorWidget << ")";
        return;
    }

    if (targetWidget == anchorWidget) {
        qWarning() << Q_FUNC_INFO << "targetWidget same as anchorWidget";
        return;
    }

    QWidget *parentWidget = targetWidget->parentWidget();
    if (parentWidget != anchorWidget && parentWidget != anchorWidget->parentWidget()) {

        QWidget *anchorParentWidget = anchorWidget->parentWidget();
        bool found = false;
        while (anchorParentWidget) {
            if (anchorParentWidget == parentWidget) {
                found = true;
                break;
            }
            anchorParentWidget = anchorParentWidget->parentWidget();
        }

        if (!found) {
            qWarning() << Q_FUNC_INFO << "targetWidget(" << targetWidget << ") not child of anchorWidget("
                       << anchorWidget << ") or anchorWidget parent's widget(" << anchorWidget->parentWidget() << ")";
            return;
        }
    }

    if ((targetAnchorLine == Left || targetAnchorLine == Right || targetAnchorLine == HorizontalCenter)
        && (anchorLine == Top || anchorLine == Bottom || anchorLine == VerticalCenter)) {
        qWarning() << Q_FUNC_INFO << "targetAnchorLine and anchorLine not in the same direction";
        return;
    }
    if ((targetAnchorLine == Top || targetAnchorLine == Bottom || targetAnchorLine == VerticalCenter)
            && (anchorLine == Left || anchorLine == Right || anchorLine == HorizontalCenter)) {
        qWarning() << Q_FUNC_INFO << "targetAnchorLine and anchorLine not in the same direction";
        return;
    }

    Q_D(AnchorManager);
    AnchorData data = d->mItems.value(targetWidget);

    if (data.mAnchorsLine.contains(targetAnchorLine)) {   // 去除旧的锚定窗口
        QWidget *oldWidget = data.mAnchorsLine[targetAnchorLine].first;
        if (d->mAnchorItems.contains(oldWidget)) {
            QList<QWidget *> values = d->mAnchorItems.value(oldWidget);
            values.removeOne(oldWidget);
            d->mAnchorItems[oldWidget] = values;
            if (values.isEmpty())
                anchorWidget->removeEventFilter(this);
        }
    } else {
        if (!d->mAnchorItems.contains(anchorWidget))
            anchorWidget->installEventFilter(this);
        QList<QWidget *> values = d->mAnchorItems[anchorWidget];
        values.append(targetWidget);
        d->mAnchorItems[anchorWidget] = values;
    }
    // 新增锚定窗口
    data.mAnchorsLine[targetAnchorLine] = qMakePair(anchorWidget, anchorLine);

    if (targetAnchorLine == Left || targetAnchorLine == Right)
        data.mAnchorsLine.remove(HorizontalCenter);
    else if (targetAnchorLine == Top || targetAnchorLine == Bottom)
        data.mAnchorsLine.remove(VerticalCenter);
    else if (targetAnchorLine == HorizontalCenter) {
        data.mAnchorsLine.remove(Left);
        data.mAnchorsLine.remove(Right);
    } else if (targetAnchorLine == VerticalCenter) {
        data.mAnchorsLine.remove(Top);
        data.mAnchorsLine.remove(Bottom);
    }
    d->mItems[targetWidget] = data;
}

void AnchorManager::setMargins(QWidget *targetWidget, const QMargins &margins)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return;

    AnchorData data = d->mItems.value(targetWidget);
    data.mMargins = margins;
    d->mItems[targetWidget] = data;
}

QMargins AnchorManager::margins(QWidget *targetWidget) const
{
    Q_D(const AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return QMargins();

    AnchorData data = d->mItems.value(targetWidget);
    return data.mMargins;
}

void AnchorManager::setTopMargin(QWidget *targetWidget, int margin)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return;

    AnchorData data = d->mItems.value(targetWidget);
    data.mMargins.setTop(margin);
    d->mItems[targetWidget] = data;
}

int AnchorManager::topMargin(QWidget *targetWidget) const
{
    Q_D(const AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return 0;

    AnchorData data = d->mItems.value(targetWidget);
    return data.mMargins.top();
}

void AnchorManager::setBottomMargin(QWidget *targetWidget, int margin)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return;

    AnchorData data = d->mItems.value(targetWidget);
    data.mMargins.setBottom(margin);
    d->mItems[targetWidget] = data;
}

int AnchorManager::bottomMargin(QWidget *targetWidget) const
{
    Q_D(const AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return 0;

    AnchorData data = d->mItems.value(targetWidget);
    return data.mMargins.bottom();
}

void AnchorManager::setLeftMargin(QWidget *targetWidget, int margin)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return;

    AnchorData data = d->mItems.value(targetWidget);
    data.mMargins.setLeft(margin);
    d->mItems[targetWidget] = data;
}

int AnchorManager::leftMargin(QWidget *targetWidget) const
{
    Q_D(const AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return 0;

    AnchorData data = d->mItems.value(targetWidget);
    return data.mMargins.left();
}

void AnchorManager::setRightMargin(QWidget *targetWidget, int margin)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return;

    AnchorData data = d->mItems.value(targetWidget);
    data.mMargins.setRight(margin);
    d->mItems[targetWidget] = data;
}

int AnchorManager::rightMargin(QWidget *targetWidget) const
{
    Q_D(const AnchorManager);
    if (targetWidget == Q_NULLPTR)
        return 0;

    AnchorData data = d->mItems.value(targetWidget);
    return data.mMargins.right();
}

void AnchorManager::update(QWidget *targetWidget)
{
    Q_D(AnchorManager);
    if (targetWidget == Q_NULLPTR || !d->mItems.contains(targetWidget))
        return;
    d->updateWidget(targetWidget);
}

bool AnchorManager::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(AnchorManager);

    QWidget *anchorWidget = qobject_cast<QWidget *>(watched);
    if (!d->mAnchorItems.contains(anchorWidget))
        return false;

    if (event->type() == QEvent::Move || event->type() == QEvent::Resize) {
        QList<QWidget *> values = d->mAnchorItems.value(anchorWidget);
        values = QSet<QWidget *>::fromList(values).toList();   // 去除重复项
        foreach (QWidget *targetWidget, values)
            d->updateWidget(targetWidget);
    }

    // 不拦截窗口消息传递
    return false;
}


