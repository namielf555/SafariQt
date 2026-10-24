#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QScrollArea>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QTabWidget>
#include <QTabBar>
#include <QListWidget>
#include <QMenu>
#include <QSizeGrip>
#include <QShortcut>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QBitmap>
#include <QRegion>
#include <QIcon>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWindow>
#include <QScreen>
#include <QClipboard>
#include <QGuiApplication>
#include <QToolTip>
#include <QCursor>
#include <QTimer>
#include <QPointer>
#include <QHash>
#include <QSettings>
#include <QSignalBlocker>
#include <QFontMetrics>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QUrlQuery>
#include <QUrl>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineHistory>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineDownloadRequest>
#include <QWebEnginePermission>
#include <QWebEngineFullScreenRequest>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>
#include <QProgressBar>
#include <QDateTime>
#include <QRegularExpression>
#include <QDir>
#include <functional>
#include <cmath>

static constexpr double kPi = 3.14159265358979323846;

// ================================================================= icons ====
enum class Ic {
    Sidebar, Back, Forward, Reload, Download, Share, Plus, Tabs, PageMenu, Search,
    Chevron, Monitor, TabGroup, Bookmark, Glasses, People, Close,
    Microphone, Camera, Location, Bell
};

static QPixmap drawIcon(Ic kind, const QColor &c) {
    const int S = 20;
    const qreal dpr = 2.0;
    QPixmap pm(int(S * dpr), int(S * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(c, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setBrush(Qt::NoBrush);

    QPainterPath path;
    switch (kind) {
    case Ic::Sidebar:
        p.drawRoundedRect(QRectF(2.5, 4.5, 15, 11), 3, 3);
        p.drawLine(QPointF(8, 4.5), QPointF(8, 15.5));
        break;
    case Ic::Back:
        p.setPen(QPen(c, 1.9, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        path.moveTo(12.5, 4.5);
        path.lineTo(7, 10);
        path.lineTo(12.5, 15.5);
        p.drawPath(path);
        break;
    case Ic::Forward:
        p.setPen(QPen(c, 1.9, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        path.moveTo(7.5, 4.5);
        path.lineTo(13, 10);
        path.lineTo(7.5, 15.5);
        p.drawPath(path);
        break;
    case Ic::Reload: {
        const qreal cx = 10, cy = 10, r = 5.6;
        p.setPen(QPen(c, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawArc(QRectF(cx - r, cy - r, 2 * r, 2 * r), 30 * 16, 300 * 16);
        const qreal a = 330.0 * kPi / 180.0;
        QPointF tip(cx + r * std::cos(a), cy - r * std::sin(a));
        QPointF dir(-std::sin(a), -std::cos(a));
        QPointF perp(-dir.y(), dir.x());
        p.drawLine(tip, tip - dir * 3.6 + perp * 3.2);
        p.drawLine(tip, tip - dir * 3.6 - perp * 3.2);
        break;
    }
    case Ic::Download:
        p.drawEllipse(QRectF(2.5, 2.5, 15, 15));
        p.drawLine(QPointF(10, 6.5), QPointF(10, 13));
        path.moveTo(7, 10.3);
        path.lineTo(10, 13.3);
        path.lineTo(13, 10.3);
        p.drawPath(path);
        break;
    case Ic::Share: {
        path.moveTo(7, 9);
        path.lineTo(5, 9);
        path.lineTo(5, 16.5);
        path.lineTo(15, 16.5);
        path.lineTo(15, 9);
        path.lineTo(13, 9);
        p.drawPath(path);
        p.drawLine(QPointF(10, 3.5), QPointF(10, 12));
        QPainterPath head;
        head.moveTo(7, 6.5);
        head.lineTo(10, 3.5);
        head.lineTo(13, 6.5);
        p.drawPath(head);
        break;
    }
    case Ic::Plus:
        p.drawLine(QPointF(10, 4.5), QPointF(10, 15.5));
        p.drawLine(QPointF(4.5, 10), QPointF(15.5, 10));
        break;
    case Ic::Tabs:
        p.drawRoundedRect(QRectF(3.5, 6.5, 10, 10), 2, 2);
        path.moveTo(6.5, 6.5);
        path.lineTo(6.5, 5.5);
        path.quadTo(6.5, 3.5, 8.5, 3.5);
        path.lineTo(14.5, 3.5);
        path.quadTo(16.5, 3.5, 16.5, 5.5);
        path.lineTo(16.5, 11.5);
        path.quadTo(16.5, 13.5, 14.5, 13.5);
        path.lineTo(13.5, 13.5);
        p.drawPath(path);
        break;
    case Ic::PageMenu:
        p.setPen(QPen(c, 1.3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawRoundedRect(QRectF(3.5, 4.5, 13, 11), 3, 3);
        p.drawLine(QPointF(6.8, 8.6), QPointF(13.2, 8.6));
        p.drawLine(QPointF(6.8, 11.4), QPointF(13.2, 11.4));
        break;
    case Ic::Search:
        p.setPen(QPen(c, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawEllipse(QRectF(3.5, 3.5, 10, 10));
        p.drawLine(QPointF(11.4, 11.4), QPointF(16.4, 16.4));
        break;
    case Ic::Chevron:
        p.setPen(QPen(c, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        path.moveTo(6, 8);
        path.lineTo(10, 12);
        path.lineTo(14, 8);
        p.drawPath(path);
        break;
    case Ic::Monitor:
        p.setBrush(c);
        p.drawRoundedRect(QRectF(3, 4, 14, 9.5), 1.8, 1.8);
        p.setBrush(Qt::NoBrush);
        p.drawLine(QPointF(7, 16.5), QPointF(13, 16.5));
        p.drawLine(QPointF(10, 13.5), QPointF(10, 16.5));
        break;
    case Ic::TabGroup:
        p.drawRoundedRect(QRectF(3, 6.5, 11, 11), 2.5, 2.5);
        path.moveTo(6, 6.5);
        path.lineTo(6, 5.5);
        path.quadTo(6, 3.5, 8, 3.5);
        path.lineTo(15, 3.5);
        path.quadTo(17, 3.5, 17, 5.5);
        path.lineTo(17, 12.5);
        path.quadTo(17, 14.5, 15, 14.5);
        path.lineTo(14, 14.5);
        p.drawPath(path);
        p.drawLine(QPointF(8.5, 9.6), QPointF(8.5, 14.4));
        p.drawLine(QPointF(6.1, 12), QPointF(10.9, 12));
        break;
    case Ic::Bookmark:
        path.moveTo(6, 3.5);
        path.lineTo(14, 3.5);
        path.lineTo(14, 16.5);
        path.lineTo(10, 13);
        path.lineTo(6, 16.5);
        path.closeSubpath();
        p.drawPath(path);
        break;
    case Ic::Glasses:
        p.drawEllipse(QRectF(2.5, 7.5, 6, 6));
        p.drawEllipse(QRectF(11.5, 7.5, 6, 6));
        p.drawLine(QPointF(8.5, 10.5), QPointF(11.5, 10.5));
        break;
    case Ic::People:
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawEllipse(QPointF(8, 7), 2.8, 2.8);
        p.drawEllipse(QPointF(14.2, 8), 2.2, 2.2);
        p.drawRoundedRect(QRectF(2.5, 11.5, 11, 5.5), 2.7, 2.7);
        p.drawRoundedRect(QRectF(12, 12.2, 5.5, 4.8), 2.2, 2.2);
        break;
    case Ic::Close:
        p.setPen(QPen(c, 1.7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawLine(QPointF(5.5, 5.5), QPointF(14.5, 14.5));
        p.drawLine(QPointF(14.5, 5.5), QPointF(5.5, 14.5));
        break;
    case Ic::Microphone: {
        p.drawRoundedRect(QRectF(7.5, 3.5, 5, 8.5), 2.5, 2.5);
        p.drawArc(QRectF(5, 6.5, 10, 8), 0, -180 * 16);
        p.drawLine(QPointF(10, 14.5), QPointF(10, 17));
        p.drawLine(QPointF(7, 17), QPointF(13, 17));
        break;
    }
    case Ic::Camera: {
        p.drawRoundedRect(QRectF(3, 6, 14, 10.5), 2, 2);
        p.drawEllipse(QPointF(10, 11.25), 2.8, 2.8);
        QPainterPath lens;
        lens.moveTo(7, 6);
        lens.lineTo(8.5, 4);
        lens.lineTo(11.5, 4);
        lens.lineTo(13, 6);
        p.drawPath(lens);
        break;
    }
    case Ic::Location: {
        path.moveTo(10, 17.5);
        path.cubicTo(6, 12, 5, 9.5, 5, 7.5);
        path.arcTo(QRectF(5, 2.5, 10, 10), 180, -180);
        path.cubicTo(15, 9.5, 14, 12, 10, 17.5);
        p.drawPath(path);
        p.drawEllipse(QPointF(10, 7.5), 2, 2);
        break;
    }
    case Ic::Bell: {
        p.drawArc(QRectF(5, 4.5, 10, 10), 0, 180 * 16);
        p.drawLine(QPointF(5, 9.5), QPointF(4, 14));
        p.drawLine(QPointF(15, 9.5), QPointF(16, 14));
        p.drawLine(QPointF(3, 14), QPointF(17, 14));
        p.drawArc(QRectF(8.5, 14, 3, 3), 0, -180 * 16);
        p.drawEllipse(QPointF(10, 3.5), 1.2, 1.2);
        break;
    }
    }
    return pm;
}

// Window-layout glyph for the green-button popup (frac = filled part, 0..1).
static QPixmap drawLayoutIcon(const QRectF &frac, const QColor &c) {
    const qreal dpr = 2.0;
    QPixmap pm(int(30 * dpr), int(22 * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(c, 1.3));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(QRectF(1.5, 1.5, 27, 19), 3, 3);
    p.setPen(Qt::NoPen);
    p.setBrush(c);
    QRectF r(2.4 + frac.x() * 25.2, 2.4 + frac.y() * 17.2, frac.width() * 25.2, frac.height() * 17.2);
    p.drawRoundedRect(r, 1.8, 1.8);
    return pm;
}

// Small colored app icon for the share popover.
static QIcon dotIcon(const QColor &c, bool circle = false) {
    QPixmap pm(32, 32);
    pm.setDevicePixelRatio(2.0);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(c);
    if (circle) p.drawEllipse(QRectF(0.5, 0.5, 15, 15));
    else p.drawRoundedRect(QRectF(0.5, 0.5, 15, 15), 4, 4);
    return QIcon(pm);
}

// ============================================================== widgets ====
// Page that opens popups in new tabs and intercepts safariqt:// links
// (add / edit / remove favorites on the Start Page).
class Page : public QWebEnginePage {
public:
    std::function<QWebEnginePage *()> onCreate;
    std::function<void(const QUrl &)> onCommand;
    std::function<void(QWebEnginePermission)> onPermission;
    std::function<void(QWebEngineFullScreenRequest)> onFullScreen;

    explicit Page(QWebEngineProfile *profile, QObject *parent = nullptr)
        : QWebEnginePage(profile, parent) {
        connect(this, &QWebEnginePage::permissionRequested, this, [this](QWebEnginePermission perm) {
            if (onPermission) onPermission(perm);
        });
        connect(this, &QWebEnginePage::fullScreenRequested, this, [this](QWebEngineFullScreenRequest req) {
            if (onFullScreen) onFullScreen(req);
        });
    }

protected:
    QWebEnginePage *createWindow(WebWindowType) override {
        return onCreate ? onCreate() : nullptr;
    }
    bool acceptNavigationRequest(const QUrl &url, NavigationType, bool) override {
        if (url.scheme() == "safariqt") {
            if (onCommand) onCommand(url);
            return false;
        }
        return true;
    }
};

// Web view that can open links (target=_blank, window.open) in a new tab.
class WebView : public QWebEngineView {
public:
    std::function<WebView *()> opener;
    std::function<void(const QUrl &)> command;

    WebView() {
        auto *pg = new Page(QWebEngineProfile::defaultProfile(), this);
        pg->onCreate = [this]() -> QWebEnginePage * {
            WebView *nv = opener ? opener() : nullptr;
            return nv ? nv->page() : nullptr;
        };
        pg->onCommand = [this](const QUrl &u) { if (command) command(u); };
        setPage(pg);
        settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
        settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
        settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);
        settings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);
    }

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType) override {
        return opener ? opener() : nullptr;
    }
};

// Area that drags the frameless window; double-click maximizes.
class DragBar : public QWidget {
public:
    std::function<void()> onDoubleClick;
    using QWidget::QWidget;

protected:
    void mousePressEvent(QMouseEvent *e) override {
        if (e->button() == Qt::LeftButton && window()->windowHandle())
            window()->windowHandle()->startSystemMove();
    }
    void mouseDoubleClickEvent(QMouseEvent *) override {
        if (onDoubleClick) onDoubleClick();
        else {
            QWidget *w = window();
            if (w->isMaximized()) w->showNormal(); else w->showMaximized();
        }
    }
};

// macOS traffic lights; glyphs appear when the pointer is over the group.
class TrafficLights : public QWidget {
public:
    std::function<void()> onClose, onMinimize, onZoom;
    std::function<void(bool)> onZoomHover;

    explicit TrafficLights(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(60, 16);
        setMouseTracking(true);
    }

protected:
    int hit(const QPoint &pt) const {
        for (int i = 0; i < 3; ++i)
            if (QLineF(QPointF(8 + 20 * i, 8), QPointF(pt)).length() <= 9) return i;
        return -1;
    }
    void setZoomHover(bool h) {
        if (h == zoomHov) return;
        zoomHov = h;
        if (onZoomHover) onZoomHover(h);
    }
    void enterEvent(QEnterEvent *) override { hover = true; animateGlyphs(1.0); }
    void leaveEvent(QEvent *) override { hover = false; setZoomHover(false); animateGlyphs(0.0); }
    void mouseMoveEvent(QMouseEvent *e) override { setZoomHover(hit(e->pos()) == 2); }
    void mousePressEvent(QMouseEvent *e) override {
        down = hit(e->pos());
        if (down < 0) e->ignore(); else update();
    }
    void mouseReleaseEvent(QMouseEvent *e) override {
        const int h = hit(e->pos());
        const int d = down;
        down = -1;
        update();
        if (d >= 0 && d == h) {
            if (d == 0 && onClose) onClose();
            else if (d == 1 && onMinimize) onMinimize();
            else if (d == 2 && onZoom) onZoom();
        }
    }
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        static const char *fill[3] = {"#ff5f57", "#febc2e", "#28c840"};
        static const char *edge[3] = {"#e2463f", "#dfa023", "#1aab29"};
        for (int i = 0; i < 3; ++i) {
            const QPointF c(8 + 20 * i, 8);
            QColor f(fill[i]);
            if (down == i) f = f.darker(115);
            p.setPen(QPen(QColor(edge[i]), 1));
            p.setBrush(f);
            p.drawEllipse(c, 6.5, 6.5);
            if (glyphAlpha <= 0.01) continue;
            const int alpha = int(140 * glyphAlpha);
            QPen gp(QColor(0, 0, 0, alpha), 1.3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            p.setPen(gp);
            if (i == 0) {
                p.drawLine(c + QPointF(-2.4, -2.4), c + QPointF(2.4, 2.4));
                p.drawLine(c + QPointF(2.4, -2.4), c + QPointF(-2.4, 2.4));
            } else if (i == 1) {
                p.drawLine(c + QPointF(-3, 0), c + QPointF(3, 0));
            } else {
                p.setPen(Qt::NoPen);
                p.setBrush(QColor(0, 0, 0, alpha));
                QPolygonF a, b;
                a << c + QPointF(-3, 3) << c + QPointF(0.8, 3) << c + QPointF(-3, -0.8);
                b << c + QPointF(3, -3) << c + QPointF(-0.8, -3) << c + QPointF(3, 0.8);
                p.drawPolygon(a);
                p.drawPolygon(b);
            }
        }
    }

private:
    void animateGlyphs(qreal target) {
        if (!hoverAnim) {
            hoverAnim = new QVariantAnimation(this);
            hoverAnim->setDuration(160);
            hoverAnim->setEasingCurve(QEasingCurve::OutQuad);
            connect(hoverAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &v) {
                glyphAlpha = v.toReal();
                update();
            });
        }
        hoverAnim->stop();
        hoverAnim->setStartValue(glyphAlpha);
        hoverAnim->setEndValue(target);
        hoverAnim->start();
    }

    bool hover = false, zoomHov = false;
    int down = -1;
    qreal glyphAlpha = 0.0;
    QVariantAnimation *hoverAnim = nullptr;
};

// Rounded popover (share sheet, green-button menu).
class Panel : public QWidget {
public:
    QVBoxLayout *body;
    explicit Panel(QWidget *parent = nullptr)
        : QWidget(parent, Qt::Popup | Qt::FramelessWindowHint) {
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_DeleteOnClose);
        auto *outer = new QVBoxLayout(this);
        outer->setContentsMargins(8, 8, 8, 8);
        auto *f = new QFrame;
        f->setObjectName("panel");
        auto *shadow = new QGraphicsDropShadowEffect(f);
        shadow->setBlurRadius(22);
        shadow->setOffset(0, 5);
        shadow->setColor(QColor(0, 0, 0, 90));
        f->setGraphicsEffect(shadow);
        outer->addWidget(f);
        body = new QVBoxLayout(f);
        body->setContentsMargins(8, 8, 8, 8);
        body->setSpacing(1);
    }

    void showAnimated() {
        setWindowOpacity(0.0);
        show();
        const QPoint orig = pos();
        move(orig.x(), orig.y() - 6);

        auto *posAnim = new QPropertyAnimation(this, "pos", this);
        posAnim->setDuration(170);
        posAnim->setEasingCurve(QEasingCurve::OutCubic);
        posAnim->setStartValue(QPoint(orig.x(), orig.y() - 6));
        posAnim->setEndValue(orig);
        posAnim->start(QAbstractAnimation::DeleteWhenStopped);

        auto *fadeAnim = new QPropertyAnimation(this, "windowOpacity", this);
        fadeAnim->setDuration(170);
        fadeAnim->setEasingCurve(QEasingCurve::OutCubic);
        fadeAnim->setStartValue(0.0);
        fadeAnim->setEndValue(1.0);
        fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }
};

struct DownloadItem {
    QPointer<QWebEngineDownloadRequest> dl;
    QString fileName;
    QString fullPath;
    qint64 receivedBytes = 0;
    qint64 totalBytes = 0;
    int state = 0; // 0: in progress, 1: completed, 2: cancelled, 3: failed
};

class DownloadsPanel : public Panel {
public:
    DownloadsPanel(QWidget *parent, const QList<DownloadItem> &items,
                   std::function<void()> onClear,
                   std::function<void(QWebEngineDownloadRequest *)> onCancel)
        : Panel(parent) {
        setFixedWidth(340);

        auto *head = new QHBoxLayout;
        head->setContentsMargins(4, 2, 4, 4);
        auto *title = new QLabel("Downloads");
        title->setObjectName("panelTitle");
        auto *clearBtn = new QPushButton("Clear");
        clearBtn->setObjectName("row");
        clearBtn->setStyleSheet("color:#0a84ff;font-size:12px;font-weight:600;padding:2px 8px;");
        connect(clearBtn, &QPushButton::clicked, this, [this, onClear]() {
            if (onClear) onClear();
            close();
        });
        head->addWidget(title);
        head->addStretch();
        head->addWidget(clearBtn);
        body->addLayout(head);

        auto *sep = new QFrame;
        sep->setObjectName("sep");
        sep->setFixedHeight(1);
        body->addWidget(sep);

        if (items.isEmpty()) {
            auto *empty = new QLabel("No Downloads");
            empty->setObjectName("dim");
            empty->setAlignment(Qt::AlignCenter);
            empty->setFixedHeight(60);
            body->addWidget(empty);
            return;
        }

        auto *scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setMaximumHeight(320);
        scroll->setStyleSheet("QScrollArea{background:transparent;border:none;}");
        scroll->viewport()->setStyleSheet("background:transparent;");

        auto *container = new QWidget;
        auto *lay = new QVBoxLayout(container);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->setSpacing(6);

        for (const auto &item : items) {
            auto *card = new QWidget;
            auto *cl = new QVBoxLayout(card);
            cl->setContentsMargins(4, 4, 4, 4);
            cl->setSpacing(3);

            auto *row1 = new QHBoxLayout;
            row1->setSpacing(8);
            auto *ic = new QLabel;
            ic->setPixmap(drawIcon(Ic::Download, QColor("#0a84ff")));
            auto *nameLbl = new QLabel(QFontMetrics(card->font()).elidedText(item.fileName, Qt::ElideMiddle, 180));
            nameLbl->setStyleSheet("font-weight:600;font-size:12px;");
            nameLbl->setToolTip(item.fullPath);
            row1->addWidget(ic);
            row1->addWidget(nameLbl, 1);

            if (item.state == 0) {
                auto *cancelBtn = new QToolButton;
                cancelBtn->setIcon(drawIcon(Ic::Close, QColor("#ff453a")));
                cancelBtn->setToolTip("Cancel");
                connect(cancelBtn, &QToolButton::clicked, this, [item, onCancel, this]() {
                    if (onCancel && item.dl) onCancel(item.dl.data());
                    close();
                });
                row1->addWidget(cancelBtn);
            } else if (item.state == 1) {
                auto *openBtn = new QPushButton("Open");
                openBtn->setObjectName("row");
                openBtn->setStyleSheet("color:#0a84ff;font-size:11px;font-weight:600;padding:2px 6px;");
                connect(openBtn, &QPushButton::clicked, [item]() {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(item.fullPath));
                });
                row1->addWidget(openBtn);
            }
            cl->addLayout(row1);

            auto *statusRow = new QHBoxLayout;
            QString info;
            if (item.state == 0) {
                if (item.totalBytes > 0) {
                    qreal mbRec = item.receivedBytes / (1024.0 * 1024.0);
                    qreal mbTot = item.totalBytes / (1024.0 * 1024.0);
                    int pct = int((item.receivedBytes * 100) / item.totalBytes);
                    info = QString("%1 MB of %2 MB (%3%)").arg(mbRec, 0, 'f', 1).arg(mbTot, 0, 'f', 1).arg(pct);
                } else {
                    qreal mbRec = item.receivedBytes / (1024.0 * 1024.0);
                    info = QString("%1 MB downloaded").arg(mbRec, 0, 'f', 1);
                }
            } else if (item.state == 1) {
                qreal mbTot = item.totalBytes > 0 ? item.totalBytes / (1024.0 * 1024.0) : 0;
                info = mbTot > 0 ? QString("Completed — %1 MB").arg(mbTot, 0, 'f', 1) : "Completed";
            } else if (item.state == 2) {
                info = "Cancelled";
            } else {
                info = "Failed";
            }
            auto *infoLbl = new QLabel(info);
            infoLbl->setObjectName("dim");
            statusRow->addWidget(infoLbl);

            if (item.state == 1) {
                auto *folderBtn = new QPushButton("Show in Folder");
                folderBtn->setObjectName("row");
                folderBtn->setStyleSheet("color:rgba(128,128,128,0.95);font-size:11px;padding:1px 4px;");
                connect(folderBtn, &QPushButton::clicked, [item]() {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(item.fullPath).absolutePath()));
                });
                statusRow->addStretch();
                statusRow->addWidget(folderBtn);
            }
            cl->addLayout(statusRow);

            if (item.state == 0 && item.totalBytes > 0) {
                auto *pbar = new QProgressBar;
                pbar->setFixedHeight(4);
                pbar->setTextVisible(false);
                pbar->setRange(0, 100);
                pbar->setValue(int((item.receivedBytes * 100) / item.totalBytes));
                pbar->setStyleSheet("QProgressBar{background:rgba(0,0,0,0.08);border:none;border-radius:2px;}"
                                    "QProgressBar::chunk{background:#0a84ff;border-radius:2px;}");
                cl->addWidget(pbar);
            }

            lay->addWidget(card);
            auto *s = new QFrame;
            s->setObjectName("sep");
            s->setFixedHeight(1);
            lay->addWidget(s);
        }

        scroll->setWidget(container);
        body->addWidget(scroll);
    }
};

struct GenericPermission {
    enum class Type {
        Microphone,
        Camera,
        CameraAndMicrophone,
        Location,
        Notifications,
        ScreenShare,
        Clipboard,
        Other
    };
    Type type = Type::Microphone;
    QUrl origin;
    std::function<void(bool)> resolver;
};

// Banner for website permission requests (e.g. microphone, camera, geolocation).
class PermissionBanner : public QWidget {
public:
    PermissionBanner(QWidget *parent, const GenericPermission &perm,
                     std::function<void(bool)> callback, bool dark)
        : QWidget(parent), m_perm(perm), m_callback(callback) {
        setAttribute(Qt::WA_DeleteOnClose);
        setFixedWidth(460);

        auto *outer = new QVBoxLayout(this);
        outer->setContentsMargins(0, 0, 0, 0);

        auto *card = new QFrame;
        card->setObjectName("permCard");
        QString cardBg = dark ? "#2c2c2e" : "#ffffff";
        QString cardBorder = dark ? "#3a3a3c" : "#d1d1d6";
        QString textCol = dark ? "#f2f2f7" : "#1c1c1e";
        QString subCol = dark ? "#8e8e93" : "#636366";

        card->setStyleSheet(QString(R"(
            #permCard {
                background: %1;
                border: 1px solid %2;
                border-radius: 14px;
            }
            QLabel#permTitle { color: %3; font-weight: 600; font-size: 13px; }
            QLabel#permDesc { color: %4; font-size: 12px; }
            QPushButton#denyBtn {
                background: %5; color: %3; border: 1px solid %2;
                border-radius: 8px; padding: 6px 14px; font-size: 12px; font-weight: 500;
            }
            QPushButton#denyBtn:hover { background: %6; }
            QPushButton#allowBtn {
                background: #0a84ff; color: #ffffff; border: none;
                border-radius: 8px; padding: 6px 16px; font-size: 12px; font-weight: 600;
            }
            QPushButton#allowBtn:hover { background: #0071e3; }
        )").arg(cardBg, cardBorder, textCol, subCol,
                dark ? "#3a3a3c" : "#e5e5ea",
                dark ? "#48484a" : "#d8d8dc"));

        auto *cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 14, 16, 14);
        cl->setSpacing(10);

        auto *topRow = new QHBoxLayout;
        topRow->setSpacing(12);

        auto *icLbl = new QLabel;
        icLbl->setFixedSize(36, 36);
        icLbl->setAlignment(Qt::AlignCenter);
        icLbl->setStyleSheet("background:rgba(10,132,255,0.14); border-radius:18px;");

        Ic iconKind = Ic::Microphone;
        QString permName = "Microphone";
        switch (m_perm.type) {
        case GenericPermission::Type::Microphone:
            iconKind = Ic::Microphone;
            permName = "Microphone";
            break;
        case GenericPermission::Type::Camera:
            iconKind = Ic::Camera;
            permName = "Camera";
            break;
        case GenericPermission::Type::CameraAndMicrophone:
            iconKind = Ic::Camera;
            permName = "Camera & Microphone";
            break;
        case GenericPermission::Type::Location:
            iconKind = Ic::Location;
            permName = "Location";
            break;
        case GenericPermission::Type::Notifications:
            iconKind = Ic::Bell;
            permName = "Notifications";
            break;
        case GenericPermission::Type::ScreenShare:
            iconKind = Ic::Monitor;
            permName = "Screen Sharing";
            break;
        case GenericPermission::Type::Clipboard:
            iconKind = Ic::Bookmark;
            permName = "Clipboard";
            break;
        default:
            iconKind = Ic::PageMenu;
            permName = "Device";
            break;
        }
        icLbl->setPixmap(drawIcon(iconKind, QColor("#0a84ff")));

        auto *textColLay = new QVBoxLayout;
        textColLay->setSpacing(2);
        auto *title = new QLabel(permName + " Permission Request");
        title->setObjectName("permTitle");

        QString host = m_perm.origin.host();
        if (host.isEmpty()) host = m_perm.origin.toString();
        if (host.startsWith("www.")) host.remove(0, 4);

        auto *desc = new QLabel(QString("\"%1\" would like to access your %2.")
                                    .arg(host, permName.toLower()));
        desc->setObjectName("permDesc");
        desc->setWordWrap(true);
        textColLay->addWidget(title);
        textColLay->addWidget(desc);

        topRow->addWidget(icLbl);
        topRow->addLayout(textColLay, 1);
        cl->addLayout(topRow);

        auto *btnRow = new QHBoxLayout;
        btnRow->setSpacing(8);
        btnRow->addStretch();
        auto *deny = new QPushButton("Don't Allow");
        deny->setObjectName("denyBtn");
        auto *allow = new QPushButton("Allow");
        allow->setObjectName("allowBtn");
        btnRow->addWidget(deny);
        btnRow->addWidget(allow);
        cl->addLayout(btnRow);

        outer->addWidget(card);

        connect(allow, &QPushButton::clicked, this, [this]() {
            dismiss(true);
        });
        connect(deny, &QPushButton::clicked, this, [this]() {
            dismiss(false);
        });

        auto *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(24);
        shadow->setOffset(0, 8);
        shadow->setColor(QColor(0, 0, 0, dark ? 160 : 70));
        setGraphicsEffect(shadow);
    }

    void startSlideIn(int targetX, int targetY) {
        m_targetX = targetX;
        m_targetY = targetY;
        move(targetX, targetY - 40);
        setWindowOpacity(0.0);
        show();
        raise();

        auto *anim = new QPropertyAnimation(this, "pos", this);
        anim->setDuration(260);
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->setStartValue(QPoint(targetX, targetY - 40));
        anim->setEndValue(QPoint(targetX, targetY));
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        auto *fadeAnim = new QPropertyAnimation(this, "windowOpacity", this);
        fadeAnim->setDuration(220);
        fadeAnim->setEasingCurve(QEasingCurve::OutCubic);
        fadeAnim->setStartValue(0.0);
        fadeAnim->setEndValue(1.0);
        fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void dismiss(bool granted) {
        if (m_dismissing) return;
        m_dismissing = true;

        auto *anim = new QPropertyAnimation(this, "pos", this);
        anim->setDuration(180);
        anim->setEasingCurve(QEasingCurve::InCubic);
        anim->setStartValue(pos());
        anim->setEndValue(QPoint(m_targetX, m_targetY - 50));

        auto *fadeAnim = new QPropertyAnimation(this, "windowOpacity", this);
        fadeAnim->setDuration(180);
        fadeAnim->setEasingCurve(QEasingCurve::InCubic);
        fadeAnim->setStartValue(windowOpacity());
        fadeAnim->setEndValue(0.0);

        connect(anim, &QPropertyAnimation::finished, this, [this, granted]() {
            if (m_perm.resolver) m_perm.resolver(granted);
            if (m_callback) m_callback(granted);
            close();
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }

protected:
    void keyPressEvent(QKeyEvent *e) override {
        if (e->key() == Qt::Key_Escape) {
            dismiss(false);
        } else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            dismiss(true);
        } else {
            QWidget::keyPressEvent(e);
        }
    }

private:
    GenericPermission m_perm;
    std::function<void(bool)> m_callback;
    int m_targetX = 0, m_targetY = 0;
    bool m_dismissing = false;
};

// ============================================================== browser ====
class Browser : public QMainWindow {
    enum class SbMode { Tabs, Bookmarks, Reading, Shared };

public:
    Browser() {
        resize(1180, 760);
        setWindowTitle("Safari");
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);   // needed for rounded window corners

        bookmarks = settings.value("bookmarks").toStringList();
        readingList = settings.value("reading").toStringList();
        favs = settings.contains("favorites") ? settings.value("favorites").toStringList()
                                              : defaultFavorites();

        initTheme();
        setStyleSheet(appStyle());

        // ---------------------------------------------------- root layout
        auto *root = new QWidget(this);
        root->setObjectName("root");
        root->setAttribute(Qt::WA_StyledBackground, true);
        auto *rootLay = new QHBoxLayout(root);
        rootLay->setContentsMargins(1, 1, 1, 1);
        rootLay->setSpacing(0);
        setCentralWidget(root);

        // ------------------------------------------------------ sidebar
        sidebarPanel = new QWidget;
        sidebarPanel->setObjectName("sidebarPanel");
        sidebarPanel->setAttribute(Qt::WA_StyledBackground, true);
        sidebarPanel->setFixedWidth(250);
        sidebarPanel->hide();
        auto *sbLay = new QVBoxLayout(sidebarPanel);
        sbLay->setContentsMargins(0, 0, 0, 0);
        sbLay->setSpacing(0);

        auto *sbHead = new DragBar;
        sbHead->setFixedHeight(54);
        sbHead->onDoubleClick = [this] { toggleFullScreen(); };
        sbHeadLay = new QHBoxLayout(sbHead);
        sbHeadLay->setContentsMargins(16, 0, 10, 0);
        sbHeadLay->setSpacing(8);
        sbHeadLay->addStretch();
        tabGroupBtn = mkBtn(Ic::TabGroup, "New Tab");
        sbHeadLay->addWidget(tabGroupBtn);
        sbLay->addWidget(sbHead);

        sidebar = new QListWidget;
        sidebar->setObjectName("sidebar");
        sidebar->setIconSize(QSize(16, 16));
        sidebar->setFrameShape(QFrame::NoFrame);
        sidebar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sbLay->addWidget(sidebar, 1);

        auto *sbBottom = new QWidget;
        auto *bl = new QVBoxLayout(sbBottom);
        bl->setContentsMargins(10, 6, 10, 12);
        bl->setSpacing(2);
        sbBookmarks = sbItem(Ic::Bookmark, "Bookmarks");
        sbReading = sbItem(Ic::Glasses, "Reading List");
        sbShared = sbItem(Ic::People, "Shared with You");
        bl->addWidget(sbBookmarks);
        bl->addWidget(sbReading);
        bl->addWidget(sbShared);
        sbLay->addWidget(sbBottom);
        rootLay->addWidget(sidebarPanel);

        // ------------------------------------------------- right column
        auto *col = new QWidget;
        auto *colLay = new QVBoxLayout(col);
        colLay->setContentsMargins(0, 0, 0, 0);
        colLay->setSpacing(0);
        rootLay->addWidget(col, 1);

        // ------------------------------------------------------ toolbar
        auto *toolbar = new DragBar;
        toolbar->setObjectName("toolbar");
        toolbar->setAttribute(Qt::WA_StyledBackground, true);
        toolbar->setFixedHeight(54);
        toolbar->onDoubleClick = [this] { toggleFullScreen(); };
        auto *tbLay = new QHBoxLayout(toolbar);
        tbLay->setContentsMargins(0, 0, 0, 0);
        tbLay->setSpacing(0);

        // window controls (live in the toolbar, or in the sidebar header)
        controls = new QWidget;
        auto *cl = new QHBoxLayout(controls);
        cl->setContentsMargins(0, 0, 0, 0);
        cl->setSpacing(0);
        lights = new TrafficLights;
        lights->onClose = [this] { close(); };
        lights->onMinimize = [this] { showMinimized(); };
        lights->onZoom = [this] { toggleFullScreen(); };
        zoomTimer = new QTimer(this);
        zoomTimer->setSingleShot(true);
        connect(zoomTimer, &QTimer::timeout, this, [this] { if (zoomHovering) showZoomPanel(); });
        lights->onZoomHover = [this](bool h) {
            zoomHovering = h;
            if (h) zoomTimer->start(450); else zoomTimer->stop();
        };
        sidebarBtn = mkBtn(Ic::Sidebar, "Show Sidebar");
        chevBtn = mkBtn(Ic::Chevron, "Sidebar options", 18, 32);
        chevBtn->setIconSize(QSize(16, 16));
        cl->addWidget(lights);
        cl->addSpacing(14);
        cl->addWidget(sidebarBtn);
        cl->addWidget(chevBtn);

        toolLeft = new QWidget;
        toolLeftLay = new QHBoxLayout(toolLeft);
        toolLeftLay->setContentsMargins(16, 0, 0, 0);
        toolLeftLay->setSpacing(2);
        backBtn = mkBtn(Ic::Back, "Back", 30, 32);
        fwdBtn = mkBtn(Ic::Forward, "Forward", 30, 32);
        toolLeftLay->addSpacing(6);
        toolLeftLay->addWidget(backBtn);
        toolLeftLay->addWidget(fwdBtn);
        toolLeftLay->addStretch();

        urlBar = new QLineEdit;
        urlBar->setFixedHeight(34);
        urlBar->setMinimumWidth(260);
        urlBar->setMaximumWidth(620);
        urlBar->setAlignment(Qt::AlignHCenter);
        urlBar->setPlaceholderText("Search or enter website name");
        urlBar->setStyleSheet(urlStyle(0));
        urlBar->installEventFilter(this);
        leadAct = urlBar->addAction(mkIcon(Ic::PageMenu), QLineEdit::LeadingPosition);
        reloadAct = urlBar->addAction(mkIcon(Ic::Reload), QLineEdit::TrailingPosition);

        toolRight = new QWidget;
        auto *rl = new QHBoxLayout(toolRight);
        rl->setContentsMargins(0, 0, 16, 0);
        rl->setSpacing(2);
        rl->addStretch();
        dlBtn = mkBtn(Ic::Download, "Show Downloads");
        dlBtn->hide();
        shareBtn = mkBtn(Ic::Share, "Share");
        newBtn = mkBtn(Ic::Plus, "New Tab");
        tabsBtn = mkBtn(Ic::Tabs, "Show Tab Overview");
        rl->addWidget(dlBtn);
        rl->addWidget(shareBtn);
        rl->addWidget(newBtn);
        rl->addWidget(tabsBtn);

        tbLay->addWidget(toolLeft);
        tbLay->addStretch(1);
        tbLay->addWidget(urlBar, 6);
        tbLay->addStretch(1);
        tbLay->addWidget(toolRight);
        colLay->addWidget(toolbar);

        // --------------------------------------------- tabs + overview
        stack = new QStackedWidget;
        stack->installEventFilter(this);   // keeps the rounded-corner clip mask in sync
        tabs = new QTabWidget;
        tabs->setDocumentMode(true);
        tabs->setTabsClosable(true);
        tabs->setMovable(true);
        tabs->setTabBarAutoHide(true);   // like Safari: tab bar only with 2+ tabs
        stack->addWidget(tabs);
        buildOverview();
        colLay->addWidget(stack, 1);

        grip = new QSizeGrip(this);
        grip->setFixedSize(16, 16);

        // Resizing fires many events; rebuild the overview grid only once things settle.
        overviewTimer = new QTimer(this);
        overviewTimer->setSingleShot(true);
        connect(overviewTimer, &QTimer::timeout, this, [this] { if (overviewActive) rebuildOverview(); });

        placeControls(false);
        setSideWidth(false);

        // ------------------------------------------------------- menus
        connect(dlBtn, &QToolButton::clicked, this, [this] { showDownloadsPanel(); });

        auto *cm = new QMenu(chevBtn);
        chevBtn->setMenu(cm);
        chevBtn->setPopupMode(QToolButton::InstantPopup);
        connect(cm, &QMenu::aboutToShow, this, [this, cm] {
            cm->clear();
            connect(cm->addAction(sidebarPanel->isVisible() ? "Hide Sidebar" : "Show Sidebar"),
                    &QAction::triggered, this, [this] { toggleSidebar(); });
            connect(cm->addAction("New Tab"), &QAction::triggered, this, [this] { openStartTab(); });
            connect(cm->addAction("Show Tab Overview"), &QAction::triggered, this, [this] { toggleOverview(); });
        });

        // ----------------------------------------------------- signals
        connect(backBtn, &QToolButton::clicked, this, [this] { if (auto v = view()) v->back(); });
        connect(fwdBtn, &QToolButton::clicked, this, [this] { if (auto v = view()) v->forward(); });
        connect(reloadAct, &QAction::triggered, this, [this] { if (auto v = view()) v->reload(); });
        connect(leadAct, &QAction::triggered, this, [this] { showPageMenu(); });
        connect(newBtn, &QToolButton::clicked, this, [this] { openStartTab(); });
        connect(tabGroupBtn, &QToolButton::clicked, this, [this] { openStartTab(); });
        connect(sidebarBtn, &QToolButton::clicked, this, [this] { toggleSidebar(); });
        connect(tabsBtn, &QToolButton::clicked, this, [this] { toggleOverview(); });
        connect(shareBtn, &QToolButton::clicked, this, [this] { showSharePanel(); });
        connect(urlBar, &QLineEdit::returnPressed, this, [this] { navigate(urlBar->text()); });

        connect(sbBookmarks, &QPushButton::clicked, this, [this] { setSbMode(SbMode::Bookmarks); });
        connect(sbReading, &QPushButton::clicked, this, [this] { setSbMode(SbMode::Reading); });
        connect(sbShared, &QPushButton::clicked, this, [this] { setSbMode(SbMode::Shared); });
        connect(sidebar, &QListWidget::itemClicked, this, [this](QListWidgetItem *it) {
            const int row = sidebar->row(it);
            if (sbMode == SbMode::Tabs) {
                if (row >= 1 && row - 1 < tabs->count()) {
                    if (overviewActive) hideOverview();
                    tabs->setCurrentIndex(row - 1);
                }
            } else {
                const QString url = it->data(Qt::UserRole).toString();
                if (!url.isEmpty() && view()) {
                    if (overviewActive) hideOverview();
                    view()->load(QUrl(url));
                }
            }
        });

        connect(tabs, &QTabWidget::tabCloseRequested, this, [this](int i) { closeTab(i); });
        connect(tabs, &QTabWidget::currentChanged, this, [this](int idx) {
            syncUi();
            animateTabSwitch(idx);
        });

        // ---------------------------------------------------- shortcuts
        new QShortcut(QKeySequence::Back, this, [this] { if (auto v = view()) v->back(); });
        new QShortcut(QKeySequence::Forward, this, [this] { if (auto v = view()) v->forward(); });
        new QShortcut(QKeySequence::Refresh, this, [this] { if (auto v = view()) v->reload(); });
        new QShortcut(QKeySequence::AddTab, this, [this] { openStartTab(); });
        new QShortcut(QKeySequence::Close, this, [this] { closeTab(tabs->currentIndex()); });
        new QShortcut(QKeySequence("Ctrl+L"), this, [this] { urlBar->setFocus(); });
        new QShortcut(QKeySequence("Ctrl+Alt+S"), this, [this] { toggleSidebar(); });
        new QShortcut(QKeySequence("Ctrl+Shift+\\"), this, [this] { toggleOverview(); });
        new QShortcut(QKeySequence(Qt::Key_Escape), this, [this] { if (overviewActive) hideOverview(); });
        new QShortcut(QKeySequence(Qt::Key_F11), this, [this] { toggleFullScreen(); });

        // ---------------------------------------------------- downloads
        connect(QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
                this, &Browser::handleDownloadRequested);

        qApp->installEventFilter(this);   // edge-resize for the frameless window

        openStartTab();
    }

protected:
    void resizeEvent(QResizeEvent *e) override {
        QMainWindow::resizeEvent(e);
        grip->move(width() - grip->width() - 3, height() - grip->height() - 3);
        if (isEffectivelyFullScreen()) grip->hide(); else { grip->show(); grip->raise(); }

        // Rounded window corners (like macOS). The window is translucent, so the
        // root's border-radius is antialiased by itself; no hard window mask.
        const int sq = isEffectivelyFullScreen() ? 1 : 0;
        if (sq != lastSquare) {   // only touch the stylesheet when the state really changes
            lastSquare = sq;
            centralWidget()->setStyleSheet(sq ? QString("#root { border-radius:0px; }") : QString());
        }
        updateCorners();
        if (activeBanner) {
            int bw = activeBanner->width();
            activeBanner->move((centralWidget()->width() - bw) / 2, 58);
        }
        if (overviewActive && overviewTimer) overviewTimer->start(60);
    }

    void changeEvent(QEvent *e) override {
        QMainWindow::changeEvent(e);
        if (e->type() == QEvent::WindowStateChange) {
            if (isEffectivelyFullScreen()) grip->hide(); else { grip->show(); grip->raise(); }
            const int sq = isEffectivelyFullScreen() ? 1 : 0;
            if (sq != lastSquare) {
                lastSquare = sq;
                centralWidget()->setStyleSheet(sq ? QString("#root { border-radius:0px; }") : QString());
            }
            updateCorners();
        }
    }

    bool eventFilter(QObject *o, QEvent *e) override {
        // ---- edge resizing (frameless window) ----
        if ((e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress)
            && o->isWidgetType() && static_cast<QWidget *>(o)->window() == this
            && !isEffectivelyFullScreen()) {
            auto *me = static_cast<QMouseEvent *>(e);
            const QPoint p = mapFromGlobal(me->globalPosition().toPoint());
            const int m = 6;
            Qt::Edges edges;
            if (p.x() <= m) edges |= Qt::LeftEdge;
            if (p.x() >= width() - m) edges |= Qt::RightEdge;
            if (p.y() <= m) edges |= Qt::TopEdge;
            if (p.y() >= height() - m) edges |= Qt::BottomEdge;

            if (e->type() == QEvent::MouseMove && me->buttons() == Qt::NoButton) {
                Qt::CursorShape shape = Qt::ArrowCursor;
                if (edges == (Qt::LeftEdge | Qt::TopEdge) || edges == (Qt::RightEdge | Qt::BottomEdge))
                    shape = Qt::SizeFDiagCursor;
                else if (edges == (Qt::RightEdge | Qt::TopEdge) || edges == (Qt::LeftEdge | Qt::BottomEdge))
                    shape = Qt::SizeBDiagCursor;
                else if (edges & (Qt::LeftEdge | Qt::RightEdge))
                    shape = Qt::SizeHorCursor;
                else if (edges & (Qt::TopEdge | Qt::BottomEdge))
                    shape = Qt::SizeVerCursor;

                if (edges && !edgeCursor) { QGuiApplication::setOverrideCursor(shape); edgeCursor = true; }
                else if (edges && edgeCursor) QGuiApplication::changeOverrideCursor(shape);
                else if (!edges && edgeCursor) { QGuiApplication::restoreOverrideCursor(); edgeCursor = false; }
            } else if (e->type() == QEvent::MouseButtonPress && me->button() == Qt::LeftButton && edges) {
                if (edgeCursor) { QGuiApplication::restoreOverrideCursor(); edgeCursor = false; }
                if (windowHandle()) windowHandle()->startSystemResize(edges);
                return true;   // don't let the web view / drag bar handle this click
            }
        }

        if (o == stack && (e->type() == QEvent::Resize || e->type() == QEvent::Show)) {
            updateStackMask();
            return false;
        }
        if (o == urlBar) {
            if (e->type() == QEvent::FocusIn) {
                // Safari shows only the host until you click; then the full URL.
                auto v = view();
                if (v && !overviewActive && !isBlank(v)) urlBar->setText(v->url().toString());
                else urlBar->clear();
                urlBar->setAlignment(Qt::AlignLeft);
                QTimer::singleShot(0, urlBar, &QLineEdit::selectAll);
            } else if (e->type() == QEvent::FocusOut) {
                showDisplay();
            }
        }
        return QMainWindow::eventFilter(o, e);
    }

private:
    // ----------------------------------------------------------- members
    QTabWidget *tabs;
    QStackedWidget *stack;
    QLineEdit *urlBar;
    QListWidget *sidebar;
    QSizeGrip *grip;
    QWidget *sidebarPanel, *controls, *toolLeft, *toolRight;
    QHBoxLayout *toolLeftLay, *sbHeadLay;
    TrafficLights *lights;
    QToolButton *backBtn, *fwdBtn, *sidebarBtn, *chevBtn, *tabGroupBtn;
    QToolButton *dlBtn, *shareBtn, *newBtn, *tabsBtn;
    QPushButton *sbBookmarks, *sbReading, *sbShared;
    QAction *leadAct, *reloadAct;
    QTimer *zoomTimer;
    bool zoomHovering = false;
    bool edgeCursor = false;
    int lastProgress = 0, lastSquare = -1;
    QTimer *overviewTimer = nullptr;

    // animations & state
    bool isCustomFullScreen = false;
    QRect savedNormalGeo;
    bool savedBeforeVideoFullscreen = false;
    QVariantAnimation *sbAnim = nullptr;
    QVariantAnimation *progAnim = nullptr;
    int currentDisplayProgress = 0;
    QVariantAnimation *dlBtnAnim = nullptr;

    // downloads & permissions
    QList<DownloadItem> downloadItems;
    QList<GenericPermission> pendingPermissions;
    QPointer<PermissionBanner> activeBanner;

    // overview
    QWidget *overviewPage = nullptr;
    QScrollArea *scroll = nullptr;
    QGridLayout *grid = nullptr;
    QLineEdit *tabSearch = nullptr;
    bool overviewActive = false;
    QHash<QObject *, QPixmap> thumbs;

    // data
    SbMode sbMode = SbMode::Tabs;
    QStringList downloads, bookmarks, readingList, favs;
    int cornerKey = -1;
    QSettings settings{"SafariQt", "Browser"};

    // theme
    bool dark = false;
    QString cBg, cBorder, cHover, cPressed, cPill, cText, cSide, cSel, cAccent, cOver;
    QColor fg, fgDis;

    // ------------------------------------------------------------- theme
    void initTheme() {
        dark = qApp->palette().window().color().lightness() < 128;
        cBg      = dark ? "#2b2b2d" : "#f6f6f6";
        cBorder  = dark ? "#1b1b1c" : "#d8d8dc";
        cHover   = dark ? "rgba(255,255,255,0.10)" : "rgba(0,0,0,0.06)";
        cPressed = dark ? "rgba(255,255,255,0.18)" : "rgba(0,0,0,0.12)";
        cPill    = dark ? "#3b3b3e" : "#e9e9ec";
        cText    = dark ? "#f2f2f4" : "#1d1d1f";
        cSide    = dark ? "#242426" : "#e4e4e7";
        cSel     = dark ? "rgba(255,255,255,0.14)" : "rgba(0,0,0,0.10)";
        cAccent  = "rgba(10,132,255,0.28)";
        cOver    = dark ? "#3a3a3c" : "#c5c5c9";
        fg       = dark ? QColor("#e8e8ea") : QColor("#4a4a50");
        fgDis    = dark ? QColor(232, 232, 234, 80) : QColor(74, 74, 80, 90);
    }

    QString appStyle() const {
        return QString(R"(
            * { font-family: "SF Pro Text", "Helvetica Neue", "Inter", "Noto Sans", sans-serif; }
            #root { background:%1; border:1px solid %2; border-radius:24px; }
            #toolbar { background:transparent; border-bottom:1px solid %2; }
            QToolButton { border:none; border-radius:8px; background:transparent; }
            QToolButton:hover { background:%3; }
            QToolButton:pressed { background:%4; }
            QToolButton:disabled { background:transparent; }
            QToolButton::menu-indicator { image:none; width:0px; }
            QTabWidget::pane { border:none; }
            QTabBar { background:%1; border-bottom:1px solid %2; }
            QTabBar::tab { background:transparent; color:%5; padding:5px 12px; margin:4px 3px;
                           min-width:120px; max-width:220px; border-radius:8px; font-size:12px; }
            QTabBar::tab:selected { background:%7; }
            QTabBar::tab:hover:!selected { background:%3; }
            #sidebarPanel { background:%6; border-right:1px solid %2;
                            border-top-left-radius:23px; border-bottom-left-radius:23px; }
            #sidebar { background:transparent; border:none; padding:6px 10px; outline:0; }
            #sidebar::item { padding:7px 8px; border-radius:8px; color:%5; }
            #sidebar::item:selected { background:%7; color:%5; }
            #sidebar::item:hover:!selected { background:%3; }
            QPushButton#sbItem { border:none; border-radius:8px; text-align:left; padding:6px 8px;
                                 color:%5; background:transparent; font-size:13px; }
            QPushButton#sbItem:hover { background:%3; }
            QPushButton#sbItem:checked { background:%7; }
            #overview { background:%8; }
            #overview QLabel { color:%5; font-size:12px; }
            QLineEdit#tabSearch { background:%3; border:1px solid transparent; border-radius:11px;
                                  padding:3px 8px; color:%5; font-size:13px; }
            #panel { background:%1; border:1px solid %2; border-radius:18px; }
            QPushButton#row { border:none; border-radius:6px; text-align:left; padding:5px 8px;
                              color:%5; background:transparent; font-size:13px; }
            QPushButton#row:hover { background:%4; }
            QLabel#dim { color:rgba(128,128,128,0.95); font-size:11px; }
            QLabel#panelTitle { color:%5; font-weight:600; font-size:13px; }
            QFrame#sep { background:%2; max-height:1px; min-height:1px; border:none; }
            QMenu { background:%1; border:1px solid %2; border-radius:8px; padding:4px; color:%5; }
            QMenu::item { padding:6px 18px; border-radius:5px; }
            QMenu::item:selected { background:%4; }
            QToolTip { color:%5; background:%1; border:1px solid %2; }
        )").arg(cBg, cBorder, cHover, cPressed, cText, cSide, cSel, cOver);
    }

    // Address pill; the blue fill is the page-load progress (like Safari).
    QString urlStyle(int p) const {
        QString bg = cPill;
        if (p > 0 && p < 100) {
            const qreal f = p / 100.0;
            const qreal f2 = qMin<qreal>(1.0, f + 0.001);
            bg = QString("qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 %1, stop:%2 %1, stop:%3 %4, stop:1 %4)")
                     .arg(cAccent).arg(f, 0, 'f', 3).arg(f2, 0, 'f', 3).arg(cPill);
        }
        return QString("QLineEdit { background:%1; border:2px solid transparent; border-radius:17px;"
                       " padding:0 8px; color:%2; font-size:13px; selection-background-color:%3; }"
                       "QLineEdit:focus { border:2px solid #0a84ff; }")
            .arg(bg, cText, cAccent);
    }

    // ----------------------------------------------------------- helpers
    QIcon mkIcon(Ic k) const {
        QIcon i;
        i.addPixmap(drawIcon(k, fg), QIcon::Normal);
        i.addPixmap(drawIcon(k, fgDis), QIcon::Disabled);
        return i;
    }
    QIcon mkIconColor(Ic k, const QColor &c) const { return QIcon(drawIcon(k, c)); }

    QToolButton *mkBtn(Ic k, const QString &tip, int w = 34, int h = 32) {
        auto *b = new QToolButton;
        b->setIcon(mkIcon(k));
        b->setIconSize(QSize(20, 20));
        b->setToolTip(tip);
        b->setFixedSize(w, h);
        return b;
    }

    QPushButton *sbItem(Ic k, const QString &text) {
        auto *b = new QPushButton(text);
        b->setObjectName("sbItem");
        b->setIcon(mkIconColor(k, QColor("#0a84ff")));
        b->setIconSize(QSize(18, 18));
        b->setCheckable(true);
        b->setFlat(true);
        return b;
    }

    static QFrame *separator() {
        auto *f = new QFrame;
        f->setObjectName("sep");
        f->setFixedHeight(1);
        return f;
    }

    static bool isBlank(const QWebEngineView *v) {
        const QUrl u = v->url();
        return u.isEmpty() || u.toString() == "about:blank" || u.scheme() == "data";
    }

    WebView *view() const { return static_cast<WebView *>(tabs->currentWidget()); }
    WebView *viewAt(int i) const { return static_cast<WebView *>(tabs->widget(i)); }

    QString titleOf(int i) const {
        WebView *v = viewAt(i);
        if (!v) return "Untitled";
        if (!v->title().isEmpty()) return v->title();
        return v->url().host().isEmpty() ? QString("Untitled") : v->url().host();
    }

    // -------------------------------------------------------- window bits
    bool isEffectivelyFullScreen() const {
        return isMaximized() || isFullScreen() || isCustomFullScreen;
    }

    void animateTabSwitch(int idx) {
        QWidget *w = tabs->widget(idx);
        if (!w) return;
        auto *eff = new QGraphicsOpacityEffect(w);
        w->setGraphicsEffect(eff);
        auto *anim = new QPropertyAnimation(eff, "opacity", this);
        anim->setDuration(170);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->setStartValue(0.15);
        anim->setEndValue(1.0);
        connect(anim, &QPropertyAnimation::finished, w, [w]() {
            w->setGraphicsEffect(nullptr);
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void animateToGeometry(const QRect &target) {
        if (target == geometry()) return;
        auto *anim = new QPropertyAnimation(this, "geometry", this);
        anim->setDuration(220);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->setStartValue(geometry());
        anim->setEndValue(target);
        connect(anim, &QPropertyAnimation::finished, this, [this]() {
            updateCorners();
        });
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void toggleFullScreen() {
        if (isMaximized() || isFullScreen() || isCustomFullScreen) {
            isCustomFullScreen = false;
            if (isFullScreen()) showNormal();
            else if (isMaximized()) showNormal();
            else {
                QRect target = savedNormalGeo.isValid() ? savedNormalGeo : QRect(100, 100, 1180, 760);
                animateToGeometry(target);
            }
            grip->show();
        } else {
            savedNormalGeo = geometry();
            // Use native showMaximized() so the window fills available workspace
            // without covering or "eating" the system taskbar/dock on KDE/Wayland/X11:
            showMaximized();
            grip->hide();
        }
        updateCorners();
    }

    void place(const QRectF &f) {
        if (f.width() >= 1.0 && f.height() >= 1.0) {
            if (!isEffectivelyFullScreen()) toggleFullScreen();
            return;
        }
        if (isEffectivelyFullScreen()) {
            isCustomFullScreen = false;
            if (isMaximized() || isFullScreen()) showNormal();
            updateCorners();
            grip->show();
        }
        const QRect a = screen()->availableGeometry();
        QRect target(a.x() + a.width() * f.x(), a.y() + a.height() * f.y(),
                     a.width() * f.width(), a.height() * f.height());
        animateToGeometry(target);
    }

    void showZoomPanel() {
        auto *p = new Panel(this);
        p->setFixedWidth(224);

        auto layoutBtn = [this, p](const QRectF &frac, std::function<void()> fn) {
            auto *b = new QToolButton;
            b->setIcon(QIcon(drawLayoutIcon(frac, fg)));
            b->setIconSize(QSize(30, 22));
            b->setFixedSize(44, 34);
            connect(b, &QToolButton::clicked, this, [p, fn] { p->close(); fn(); });
            return b;
        };
        auto section = [p](const QString &title) {
            auto *l = new QLabel(title);
            l->setObjectName("dim");
            p->body->addWidget(l);
        };
        auto rowOf = [this, layoutBtn, p](std::initializer_list<QRectF> fracs) {
            auto *h = new QHBoxLayout;
            h->setContentsMargins(0, 2, 0, 4);
            h->setSpacing(2);
            for (const QRectF &f : fracs)
                h->addWidget(layoutBtn(f, [this, f] { place(f); }));
            p->body->addLayout(h);
        };

        section("Move & Resize");
        rowOf({QRectF(0, 0, .5, 1), QRectF(.5, 0, .5, 1), QRectF(0, 0, 1, .5), QRectF(0, .5, 1, .5)});
        p->body->addWidget(separator());
        section("Fill & Arrange");
        rowOf({QRectF(0, 0, 1, 1), QRectF(.17, .17, .66, .66), QRectF(0, 0, .66, 1), QRectF(.34, 0, .66, 1)});
        p->body->addWidget(separator());

        auto *fsb = new QPushButton;
        fsb->setObjectName("row");
        fsb->setFixedHeight(28);
        auto *fl = new QHBoxLayout(fsb);
        fl->setContentsMargins(8, 0, 8, 0);
        auto *l1 = new QLabel(isEffectivelyFullScreen() ? "Exit Full Screen" : "Full Screen");
        auto *l2 = new QLabel(QString::fromUtf8("\xE2\x80\xBA"));
        l1->setAttribute(Qt::WA_TransparentForMouseEvents);
        l2->setAttribute(Qt::WA_TransparentForMouseEvents);
        fl->addWidget(l1);
        fl->addStretch();
        fl->addWidget(l2);
        connect(fsb, &QPushButton::clicked, this, [this, p] { p->close(); toggleFullScreen(); });
        p->body->addWidget(fsb);

        p->adjustSize();
        p->move(lights->mapToGlobal(QPoint(-12, lights->height() + 4)));
        p->showAnimated();
    }

    // ----------------------------------------------------------- sidebar
    // Clip the content area so the web view follows the window's rounded bottom
    // corners (inner radius = 24 - 1px border). The bottom-left corner stays
    // square when the sidebar is open, because the sidebar owns that corner.
    void updateStackMask() {
        if (!stack) return;
        if (isEffectivelyFullScreen()) { stack->clearMask(); return; }
        const qreal R = 23;
        const QSize sz = stack->size();
        if (sz.isEmpty()) return;
        const QRectF r(QPointF(0, 0), QSizeF(sz));
        QBitmap bm(sz);
        bm.fill(Qt::color0);
        QPainter p(&bm);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::color1);
        p.drawRoundedRect(r, R, R);                                    // rounded bottom corners
        p.drawRect(QRectF(0, 0, r.width(), R + 1));                    // square top corners
        if (sidebarPanel && !sidebarPanel->isHidden())
            p.drawRect(QRectF(0, r.height() - R, R, R));               // square bottom-left
        p.end();
        stack->setMask(bm);
    }

    // Children that touch the window corners must follow the rounded shape,
    // otherwise their square corners show up as a "corner bug".
    void updateCorners() {
        if (!overviewPage || !sidebarPanel) return;
        updateStackMask();
        const bool sq = isEffectivelyFullScreen();
        const bool sb = !sidebarPanel->isHidden();
        const int key = (sq ? 2 : 0) + (sb ? 1 : 0);
        if (key == cornerKey) return;
        cornerKey = key;
        const int r = sq ? 0 : 23;
        overviewPage->setStyleSheet(
            QString("#overview { border-bottom-right-radius:%1px; border-bottom-left-radius:%2px; }")
                .arg(r).arg(sb ? 0 : r));
        sidebarPanel->setStyleSheet(
            QString("#sidebarPanel { border-top-left-radius:%1px; border-bottom-left-radius:%1px; }").arg(r));
    }

    void placeControls(bool inSidebar) {
        if (auto *pw = controls->parentWidget())
            if (pw->layout()) pw->layout()->removeWidget(controls);
        if (inSidebar) sbHeadLay->insertWidget(0, controls);
        else toolLeftLay->insertWidget(0, controls);
        controls->show();
    }

    void setSideWidth(bool sidebarOpen) {
        const int w = sidebarOpen ? 170 : 250;
        toolLeft->setFixedWidth(w);
        toolRight->setFixedWidth(w);
    }

    void toggleSidebar() {
        if (!sidebarPanel) return;
        if (sbAnim && sbAnim->state() == QAbstractAnimation::Running) {
            sbAnim->stop();
        }

        const bool opening = !sidebarPanel->isVisible();
        if (!sbAnim) {
            sbAnim = new QVariantAnimation(this);
            sbAnim->setDuration(220);
            sbAnim->setEasingCurve(QEasingCurve::OutCubic);
            connect(sbAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &val) {
                int w = val.toInt();
                sidebarPanel->setFixedWidth(w);
                qreal frac = qBound(0.0, w / 250.0, 1.0);
                int toolW = 250 - int(frac * 80);
                toolLeft->setFixedWidth(toolW);
                toolRight->setFixedWidth(toolW);
                updateCorners();
            });
            connect(sbAnim, &QVariantAnimation::finished, this, [this]() {
                if (sidebarPanel->width() <= 0) {
                    sidebarPanel->hide();
                    placeControls(false);
                } else {
                    placeControls(true);
                }
                updateCorners();
            });
        }

        if (opening) {
            sidebarPanel->setFixedWidth(0);
            sidebarPanel->show();
            placeControls(true);
            refreshSidebar();
            sbAnim->setStartValue(0);
            sbAnim->setEndValue(250);
            sidebarBtn->setToolTip("Hide sidebar");
        } else {
            sbAnim->setStartValue(sidebarPanel->width());
            sbAnim->setEndValue(0);
            sidebarBtn->setToolTip("Show Sidebar");
        }
        sbAnim->start();
    }

    void setSbMode(SbMode m) {
        sbMode = (sbMode == m) ? SbMode::Tabs : m;
        sbBookmarks->setChecked(sbMode == SbMode::Bookmarks);
        sbReading->setChecked(sbMode == SbMode::Reading);
        sbShared->setChecked(sbMode == SbMode::Shared);
        refreshSidebar();
    }

    static QString enc(const QString &title, const QString &url) {
        return title + QChar(0x1f) + url;
    }

    void fillLinks(const QStringList &list, const QString &emptyText) {
        if (list.isEmpty()) {
            auto *it = new QListWidgetItem(emptyText);
            it->setFlags(Qt::NoItemFlags);
            sidebar->addItem(it);
            return;
        }
        for (const QString &e : list) {
            const QStringList parts = e.split(QChar(0x1f));
            auto *it = new QListWidgetItem(parts.value(0));
            it->setData(Qt::UserRole, parts.value(1));
            it->setToolTip(parts.value(1));
            sidebar->addItem(it);
        }
    }

    void refreshSidebar() {
        if (!sidebarPanel->isVisible()) return;
        QSignalBlocker block(sidebar);
        sidebar->clear();
        switch (sbMode) {
        case SbMode::Tabs: {
            const int n = tabs->count();
            auto *h = new QListWidgetItem(mkIconColor(Ic::Monitor, QColor("#0a84ff")),
                                          QString("%1 Tab%2").arg(n).arg(n == 1 ? "" : "s"));
            QFont f = h->font();
            f.setBold(true);
            h->setFont(f);
            h->setFlags(Qt::ItemIsEnabled);
            sidebar->addItem(h);
            for (int i = 0; i < n; ++i)
                sidebar->addItem(new QListWidgetItem(tabs->tabIcon(i), titleOf(i)));
            sidebar->setCurrentRow(tabs->currentIndex() + 1);
            break;
        }
        case SbMode::Bookmarks: fillLinks(bookmarks, "No Bookmarks"); break;
        case SbMode::Reading:   fillLinks(readingList, "No Items in Reading List"); break;
        case SbMode::Shared:    fillLinks({}, "Nothing Shared with You"); break;
        }
    }

    void addLink(QStringList &list, const char *key) {
        auto *v = view();
        if (!v || isBlank(v)) return;
        const QString e = enc(titleOf(tabs->currentIndex()), v->url().toString());
        if (!list.contains(e)) list.append(e);
        settings.setValue(key, list);
        refreshSidebar();
    }

    // ------------------------------------------------------ share popover
    void showSharePanel() {
        auto *v = view();
        if (!v || isBlank(v)) return;
        const QString title = titleOf(tabs->currentIndex());
        const QString url = v->url().toString();

        auto *p = new Panel(this);
        p->setFixedWidth(270);

        auto *hw = new QWidget;
        auto *hl = new QHBoxLayout(hw);
        hl->setContentsMargins(4, 4, 4, 6);
        hl->setSpacing(10);
        auto *ic = new QLabel;
        ic->setFixedSize(40, 40);
        ic->setAlignment(Qt::AlignCenter);
        ic->setStyleSheet("background:white; border-radius:8px;");
        QPixmap fav = tabs->tabIcon(tabs->currentIndex()).pixmap(28, 28);
        if (!fav.isNull()) ic->setPixmap(fav);
        auto *tl = new QVBoxLayout;
        tl->setSpacing(0);
        auto *t1 = new QLabel(QFontMetrics(font()).elidedText(title, Qt::ElideRight, 190));
        t1->setObjectName("panelTitle");
        auto *t2 = new QLabel(v->url().host().startsWith("www.") ? v->url().host().mid(4) : v->url().host());
        t2->setObjectName("dim");
        tl->addWidget(t1);
        tl->addWidget(t2);
        hl->addWidget(ic);
        hl->addLayout(tl, 1);
        p->body->addWidget(hw);
        p->body->addWidget(separator());

        const QColor gray("#8e8e93");
        auto na = [] { QToolTip::showText(QCursor::pos(), "Not available on this system"); };
        auto mail = [title, url] {
            const QString s = QString::fromUtf8(QUrl::toPercentEncoding(title));
            const QString b = QString::fromUtf8(QUrl::toPercentEncoding(url));
            QDesktopServices::openUrl(QUrl("mailto:?subject=" + s + "&body=" + b));
        };
        auto row = [this, p](const QIcon &icn, const QString &text, std::function<void()> fn) {
            auto *b = new QPushButton(text);
            b->setObjectName("row");
            b->setIcon(icn);
            b->setIconSize(QSize(16, 16));
            connect(b, &QPushButton::clicked, this, [p, fn] { p->close(); fn(); });
            p->body->addWidget(b);
        };

        row(dotIcon(gray), "Add to Reading List", [this] { addLink(readingList, "reading"); });
        row(dotIcon(gray), "Add Bookmark", [this] { addLink(bookmarks, "bookmarks"); });
        row(dotIcon(gray), "New Quick Note", na);
        row(dotIcon(gray), "Add to Dock", na);
        row(dotIcon(QColor("#4aa3ff"), true), "AirDrop", na);
        row(dotIcon(QColor("#1e90ff")), "Email This Page", mail);
        row(dotIcon(QColor("#1e90ff")), "Email Link to This Page", mail);
        row(dotIcon(QColor("#34c759")), "Messages", na);
        row(dotIcon(QColor("#ffd60a")), "Notes", na);
        row(dotIcon(QColor("#ff375f")), "Open in News", na);
        row(dotIcon(QColor("#bf5af2")), "Freeform", na);
        row(dotIcon(QColor("#d1d1d6")), "Reminders", na);
        p->body->addWidget(separator());
        row(dotIcon(gray), QString::fromUtf8("Edit Extensions\xE2\x80\xA6"), na);

        p->adjustSize();
        const QPoint g = shareBtn->mapToGlobal(QPoint(shareBtn->width(), shareBtn->height() + 6));
        p->move(g.x() - p->width() + 8, g.y());
        p->showAnimated();
    }

    void showPageMenu() {
        auto *v = view();
        if (!v || isBlank(v) || overviewActive) return;
        QMenu m(this);
        connect(m.addAction("Reload Page"), &QAction::triggered, this, [v] { v->reload(); });
        m.addSeparator();
        connect(m.addAction("Zoom In"), &QAction::triggered, this, [v] { v->setZoomFactor(v->zoomFactor() + 0.1); });
        connect(m.addAction("Zoom Out"), &QAction::triggered, this, [v] { v->setZoomFactor(qMax(0.3, v->zoomFactor() - 0.1)); });
        connect(m.addAction("Actual Size"), &QAction::triggered, this, [v] { v->setZoomFactor(1.0); });
        m.addSeparator();
        connect(m.addAction("Pin to Start Page"), &QAction::triggered, this, [this] { pinCurrent(); });
        connect(m.addAction("Copy Link"), &QAction::triggered, this, [v] {
            QGuiApplication::clipboard()->setText(v->url().toString());
        });
        m.exec(urlBar->mapToGlobal(QPoint(0, urlBar->height() + 4)));
    }

    // -------------------------------------------------------- tab overview
    void buildOverview() {
        overviewPage = new QWidget;
        overviewPage->setObjectName("overview");
        overviewPage->setAttribute(Qt::WA_StyledBackground, true);
        auto *l = new QVBoxLayout(overviewPage);
        l->setContentsMargins(24, 14, 24, 24);
        l->setSpacing(14);

        auto *top = new QHBoxLayout;
        top->addStretch();
        tabSearch = new QLineEdit;
        tabSearch->setObjectName("tabSearch");
        tabSearch->setPlaceholderText("Search Tabs");
        tabSearch->setFixedWidth(190);
        tabSearch->addAction(mkIcon(Ic::Search), QLineEdit::LeadingPosition);
        top->addWidget(tabSearch);
        l->addLayout(top);

        scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setStyleSheet("QScrollArea{background:transparent;border:none;} #overviewHost{background:transparent;}");
        scroll->viewport()->setStyleSheet("background:transparent;");
        auto *host = new QWidget;
        host->setObjectName("overviewHost");
        grid = new QGridLayout(host);
        grid->setContentsMargins(0, 0, 0, 0);
        grid->setSpacing(24);
        grid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        scroll->setWidget(host);
        l->addWidget(scroll, 1);

        connect(tabSearch, &QLineEdit::textChanged, this, [this] { rebuildOverview(); });
        stack->addWidget(overviewPage);
    }

    QWidget *makeCard(int i) {
        WebView *v = viewAt(i);
        auto *card = new QWidget;
        card->setFixedWidth(250);
        auto *cl = new QVBoxLayout(card);
        cl->setContentsMargins(0, 0, 0, 0);
        cl->setSpacing(4);

        auto *head = new QHBoxLayout;
        head->setContentsMargins(0, 0, 0, 0);
        head->setSpacing(4);
        auto *x = new QToolButton;
        x->setIcon(mkIcon(Ic::Close));
        x->setIconSize(QSize(14, 14));
        x->setFixedSize(20, 20);
        connect(x, &QToolButton::clicked, this, [this, i] { closeTab(i); });
        auto *fav = new QLabel;
        QPixmap fp = tabs->tabIcon(i).pixmap(14, 14);
        if (!fp.isNull()) fav->setPixmap(fp);
        auto *title = new QLabel(QFontMetrics(font()).elidedText(titleOf(i), Qt::ElideRight, 150));
        auto *spacer = new QWidget;
        spacer->setFixedWidth(20);
        head->addWidget(x);
        head->addStretch();
        head->addWidget(fav);
        head->addWidget(title);
        head->addStretch();
        head->addWidget(spacer);
        cl->addLayout(head);

        auto *thumb = new QToolButton;
        thumb->setFixedSize(250, 170);
        thumb->setIconSize(QSize(250, 170));
        thumb->setStyleSheet("QToolButton{background:white;border:1px solid rgba(0,0,0,0.18);border-radius:14px;}");
        if (thumbs.contains(v)) thumb->setIcon(QIcon(thumbs.value(v)));
        connect(thumb, &QToolButton::clicked, this, [this, i] {
            tabs->setCurrentIndex(i);
            hideOverview();
        });
        cl->addWidget(thumb);
        return card;
    }

    void rebuildOverview() {
        if (!grid) return;
        while (QLayoutItem *it = grid->takeAt(0)) {
            if (it->widget()) it->widget()->deleteLater();
            delete it;
        }
        const QString q = tabSearch->text().trimmed();
        const int cols = qMax(1, (stack->width() - 48 + 24) / 274);
        int n = 0;
        for (int i = 0; i < tabs->count(); ++i) {
            if (!q.isEmpty() && !titleOf(i).contains(q, Qt::CaseInsensitive)
                && !viewAt(i)->url().toString().contains(q, Qt::CaseInsensitive))
                continue;
            grid->addWidget(makeCard(i), n / cols, n % cols);
            ++n;
        }
        if (q.isEmpty()) {
            auto *plus = new QToolButton;
            plus->setFixedSize(250, 170);
            plus->setIcon(mkIconColor(Ic::Plus, QColor(0, 0, 0, 130)));
            plus->setIconSize(QSize(48, 48));
            plus->setStyleSheet("QToolButton{background:rgba(0,0,0,0.10);border:none;border-radius:14px;}"
                                "QToolButton:hover{background:rgba(0,0,0,0.16);}");
            connect(plus, &QToolButton::clicked, this, [this] {
                hideOverview();
                openStartTab();
            });
            auto *wrap = new QVBoxLayout;
            wrap->setContentsMargins(0, 24, 0, 0);   // line up with thumbnails (title row height)
            wrap->addWidget(plus);
            auto *w = new QWidget;
            w->setLayout(wrap);
            grid->addWidget(w, n / cols, n % cols);
        }
    }

    void animateStackTransition(QWidget *target) {
        if (!stack || stack->currentWidget() == target) return;
        auto *eff = new QGraphicsOpacityEffect(target);
        target->setGraphicsEffect(eff);
        stack->setCurrentWidget(target);

        auto *fade = new QPropertyAnimation(eff, "opacity", this);
        fade->setDuration(180);
        fade->setStartValue(0.0);
        fade->setEndValue(1.0);
        fade->setEasingCurve(QEasingCurve::OutCubic);
        connect(fade, &QPropertyAnimation::finished, target, [target]() {
            target->setGraphicsEffect(nullptr);
        });
        fade->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void showOverview() {
        if (auto v = view()) captureThumb(v);
        overviewActive = true;
        animateStackTransition(overviewPage);
        tabSearch->clear();
        tabsBtn->setToolTip("Hide Tab Overview");
        showDisplay();
        updateLeadIcon();
        rebuildOverview();
    }

    void hideOverview() {
        overviewActive = false;
        animateStackTransition(tabs);
        tabsBtn->setToolTip("Show Tab Overview");
        showDisplay();
        updateLeadIcon();
    }

    void toggleOverview() {
        if (overviewActive) hideOverview(); else showOverview();
    }

    void captureThumb(WebView *v) {
        if (!v || !v->isVisible() || v->width() < 50) return;
        QPixmap p = v->grab();
        if (p.isNull()) return;
        thumbs[v] = p.scaledToWidth(500, Qt::SmoothTransformation).copy(0, 0, 500, 340);
    }

    // ------------------------------------------------------------ favorites
    static QStringList defaultFavorites() {
        return {
            enc("Feature Tests", "safariqt://test"),
            enc("Apple", "https://www.apple.com"),
            enc("iCloud", "https://www.icloud.com"),
            enc("Google", "https://www.google.com"),
            enc("Wikipedia", "https://www.wikipedia.org"),
            enc("YouTube", "https://www.youtube.com"),
            enc("GitHub", "https://github.com"),
            enc("Reddit", "https://www.reddit.com"),
            enc("X", "https://x.com"),
        };
    }

    void saveFavs() { settings.setValue("favorites", favs); }

    QString startPageHtml(bool edit) const {
        static const QHash<QString, QString> brand = {
            {"safariqt.local", "background:#0a84ff;color:#fff"},
            {"apple.com", "background:#555;color:#fff"},
            {"icloud.com", "background:#3b9bf0;color:#fff"},
            {"yahoo.com", "background:#6001d2;color:#fff"},
            {"bing.com", "background:#0c84d8;color:#fff"},
            {"google.com", "background:#fff;color:#4285f4"},
            {"wikipedia.org", "background:#fff;color:#000"},
            {"facebook.com", "background:#1877f2;color:#fff"},
            {"x.com", "background:#000;color:#fff"},
            {"linkedin.com", "background:#0a66c2;color:#fff"},
            {"youtube.com", "background:#f00;color:#fff"},
            {"github.com", "background:#24292f;color:#fff"},
            {"reddit.com", "background:#ff4500;color:#fff"},
        };
        static const char *pal[] = {"#ff9500", "#34c759", "#5856d6", "#ff2d55",
                                    "#00a3a3", "#af52de", "#007aff", "#a2845e"};

        QString tiles;
        for (int i = 0; i < favs.size(); ++i) {
            const QStringList parts = favs.at(i).split(QChar(0x1f));
            const QString title = parts.value(0), url = parts.value(1);
            QString host = QUrl(url).host();
            if (host.startsWith("www.")) host.remove(0, 4);
            QString style;
            for (auto it = brand.constBegin(); it != brand.constEnd(); ++it)
                if (host.endsWith(it.key())) { style = it.value(); break; }
            if (style.isEmpty())
                style = QString("background:%1;color:#fff").arg(pal[qHash(host) % 8]);
            const QString letter = title.isEmpty() ? QString("?") : title.left(1).toUpper();
            tiles += QString("<div class=tile><a class=t href=\"%1\"><i style=\"%2\">%3</i><span>%4</span></a>"
                             "<a class=x href=\"safariqt://remove?i=%5\">&#10005;</a>"
                             "<a class=p href=\"safariqt://edit?i=%5\">&#9998;</a></div>")
                         .arg(url.toHtmlEscaped(), style, letter.toHtmlEscaped(), title.toHtmlEscaped())
                         .arg(i);
        }
        tiles += "<a class=tile id=add href=\"safariqt://add\"><i>+</i><span>Add Favorite</span></a>";

        QString html = QString::fromUtf8(R"HTML(<!doctype html><html><head><meta charset="utf-8"><title>Start Page</title>
<style>
:root{color-scheme:light dark;--bg:#fff;--fg:#1d1d1f;--dim:#6e6e73;--chip:rgba(0,0,0,.07)}
@media (prefers-color-scheme:dark){:root{--bg:#1c1c1e;--fg:#f5f5f7;--dim:#98989d;--chip:rgba(255,255,255,.13)}}
body{margin:0;background:var(--bg);color:var(--fg);font-family:-apple-system,"SF Pro Text","Helvetica Neue",Inter,sans-serif}
main{max-width:780px;margin:0 auto;padding:56px 24px}
.top{display:flex;align-items:center;justify-content:space-between;margin-bottom:24px}
h2{font-size:20px;font-weight:700;margin:0}
.edit{font-size:13px;color:#0a84ff;text-decoration:none;padding:6px 16px;border-radius:999px;background:var(--chip)}
.grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(108px,1fr));gap:28px 12px}
.tile,.t{display:flex;flex-direction:column;align-items:center;gap:8px;text-decoration:none;color:var(--dim);font-size:11px}
.tile{position:relative}
i{width:68px;height:68px;border-radius:20px;display:flex;align-items:center;justify-content:center;
  font:700 28px/1 -apple-system,Helvetica,Arial,sans-serif;font-style:normal;box-shadow:0 1px 3px rgba(0,0,0,.28)}
span{max-width:96px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
#add{display:none}
#add i{background:var(--chip);color:var(--dim);box-shadow:none;font-weight:300;font-size:36px}
.x,.p{display:none;position:absolute;top:-6px;width:22px;height:22px;border-radius:50%;align-items:center;
  justify-content:center;font-size:12px;text-decoration:none;color:#fff;background:#8e8e93;box-shadow:0 1px 2px rgba(0,0,0,.3)}
.x{left:calc(50% - 42px);background:#ff453a}
.p{left:calc(50% + 20px)}
.editing #add,.editing .x,.editing .p{display:flex}
.editing .t{pointer-events:none}
</style></head><body class="@EDIT@"><main>
<div class="top"><h2>Favorites</h2>
<a class="edit" href="#" onclick="var b=document.body;b.classList.toggle('editing');this.textContent=b.classList.contains('editing')?'Done':'Edit';return false">@LABEL@</a></div>
<div class="grid">@TILES@</div></main></body></html>)HTML");
        html.replace("@TILES@", tiles);
        html.replace("@EDIT@", edit ? "editing" : "");
        html.replace("@LABEL@", edit ? "Done" : "Edit");
        return html;
    }

    // Re-render every tab that is still showing the Start Page.
    void refreshStartPages(bool edit) {
        for (int i = 0; i < tabs->count(); ++i) {
            WebView *v = viewAt(i);
            if (v->property("start").toBool() && isBlank(v))
                v->setHtml(startPageHtml(edit), QUrl("about:blank"));
        }
    }

    QString testPageHtml() const {
        return QString::fromUtf8(R"HTML(<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Feature Tests</title>
<style>
:root { color-scheme: light dark; --bg:#fbfbfd; --card:#ffffff; --card-border:rgba(0,0,0,0.1); --fg:#1d1d1f; --sub:#6e6e73; }
@media (prefers-color-scheme: dark) {
  :root { --bg:#1c1c1e; --card:#2c2c2e; --card-border:rgba(255,255,255,0.12); --fg:#f5f5f7; --sub:#98989d; }
}
* { box-sizing: border-box; }
body { margin:0; background:var(--bg); color:var(--fg); font-family:-apple-system,"SF Pro Text","Helvetica Neue",Inter,sans-serif; padding:48px 24px; }
.container { max-width:680px; margin:0 auto; }
h1 { font-size:26px; font-weight:700; margin:0 0 8px 0; }
p.desc { font-size:14px; color:var(--sub); margin:0 0 32px 0; }
.card { background:var(--card); border:1px solid var(--card-border); border-radius:16px; padding:22px; margin-bottom:20px; box-shadow:0 2px 8px rgba(0,0,0,0.04); }
h3 { margin:0 0 10px 0; font-size:16px; font-weight:600; display:flex; align-items:center; gap:8px; }
p { font-size:13px; color:var(--sub); margin:0 0 16px 0; line-height:1.5; }
button { background:#0a84ff; color:white; border:none; border-radius:8px; padding:9px 18px; font-size:13px; font-weight:600; cursor:pointer; transition:all 0.15s ease; }
button:hover { background:#0071e3; transform:translateY(-1px); }
button:active { transform:translateY(0); }
.status { margin-top:14px; padding:10px 14px; border-radius:8px; font-size:13px; font-weight:500; display:none; }
.status.success { display:block; background:rgba(52,199,89,0.15); color:#34c759; }
.status.error { display:block; background:rgba(255,69,58,0.15); color:#ff453a; }
.status.info { display:block; background:rgba(10,132,255,0.15); color:#0a84ff; }
</style>
</head>
<body>
<div class="container">
  <h1>SafariQt Feature Testing</h1>
  <p class="desc">Test microphone permissions, media streams, and automatic file downloading.</p>

  <div class="card">
    <h3>🎤 Microphone Permission</h3>
    <p>Click below to request microphone access using <code>navigator.mediaDevices.getUserMedia</code>. The browser will present an animated permission banner allowing you to Allow or Don't Allow.</p>
    <button onclick="requestMic()">Request Microphone Access</button>
    <div id="micStatus" class="status"></div>
  </div>

  <div class="card">
    <h3>📷 Camera Permission</h3>
    <p>Click below to test camera permission prompt and video device access.</p>
    <button onclick="requestCam()">Request Camera Access</button>
    <div id="camStatus" class="status"></div>
  </div>

  <div class="card">
    <h3>⬇️ File Download</h3>
    <p>Test downloading a file. The file is saved directly to your Downloads folder without blocking modal prompts, with non-conflicting numbering and animated progress in the toolbar.</p>
    <button onclick="downloadSample()">Download Sample File</button>
    <div id="dlStatus" class="status"></div>
  </div>
</div>

<script>
function requestMic() {
  const s = document.getElementById('micStatus');
  s.className = 'status info';
  s.textContent = 'Requesting microphone permission...';
  navigator.mediaDevices.getUserMedia({ audio: true })
    .then(stream => {
      s.className = 'status success';
      s.textContent = '✅ Microphone permission granted! Audio stream connected.';
    })
    .catch(err => {
      s.className = 'status error';
      s.textContent = '❌ Microphone access error: ' + err.name + ' - ' + err.message;
    });
}

function requestCam() {
  const s = document.getElementById('camStatus');
  s.className = 'status info';
  s.textContent = 'Requesting camera permission...';
  navigator.mediaDevices.getUserMedia({ video: true })
    .then(stream => {
      s.className = 'status success';
      s.textContent = '✅ Camera permission granted! Video stream connected.';
    })
    .catch(err => {
      s.className = 'status error';
      s.textContent = '❌ Camera access error: ' + err.name + ' - ' + err.message;
    });
}

function downloadSample() {
  const s = document.getElementById('dlStatus');
  s.className = 'status info';
  s.textContent = 'Initiating download...';
  const blob = new Blob(["SafariQt Sample Download File\nGenerated: " + new Date().toString() + "\nAll systems operational.\n"], { type: "text/plain" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'SafariQt-Download.txt';
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  setTimeout(() => URL.revokeObjectURL(url), 1000);
  s.className = 'status success';
  s.textContent = '✅ Download started! Look at the download icon in the toolbar.';
}
</script>
</body>
</html>)HTML");
    }

    void openTestTab() {
        if (overviewActive) hideOverview();
        WebView *v = newTab(QUrl("safariqt://test"));
        v->setHtml(testPageHtml(), QUrl("https://safariqt.local/test"));
        int idx = tabs->indexOf(v);
        if (idx >= 0) tabs->setTabText(idx, "Feature Tests");
        showDisplay();
        updateLeadIcon();
        v->setFocus();
    }

    // safariqt://add | edit?i=N | remove?i=N, sent by the Start Page.
    void handleCommand(const QUrl &u) {
        const QString cmd = u.host();
        const int i = QUrlQuery(u).queryItemValue("i").toInt();
        QTimer::singleShot(0, this, [this, cmd, i] {
            if (cmd == "add") {
                editFavorite(-1);
            } else if (cmd == "edit") {
                editFavorite(i);
            } else if (cmd == "remove") {
                if (i >= 0 && i < favs.size()) {
                    favs.removeAt(i);
                    saveFavs();
                    refreshStartPages(true);
                }
            }
        });
    }

    void editFavorite(int idx) {
        QString title, url;
        if (idx >= 0 && idx < favs.size()) {
            const QStringList p = favs.at(idx).split(QChar(0x1f));
            title = p.value(0);
            url = p.value(1);
        }
        QDialog dlg(this);
        dlg.setWindowTitle(idx >= 0 ? "Edit Favorite" : "Add Favorite");
        dlg.setMinimumWidth(380);
        auto *form = new QFormLayout(&dlg);
        auto *tEdit = new QLineEdit(title);
        auto *uEdit = new QLineEdit(url);
        uEdit->setPlaceholderText("example.com");
        form->addRow("Title", tEdit);
        form->addRow("Address", uEdit);
        auto *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        form->addRow(bb);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        if (dlg.exec() != QDialog::Accepted) return;

        const QString addr = uEdit->text().trimmed();
        if (addr.isEmpty()) return;
        const QUrl qu = QUrl::fromUserInput(addr);
        QString t = tEdit->text().trimmed();
        if (t.isEmpty()) {
            t = qu.host();
            if (t.startsWith("www.")) t.remove(0, 4);
        }
        const QString e = enc(t, qu.toString());
        if (idx >= 0 && idx < favs.size()) favs[idx] = e; else favs.append(e);
        saveFavs();
        refreshStartPages(true);   // stay in edit mode
    }

    // "Pin to Start Page" from the page menu.
    void pinCurrent() {
        auto *v = view();
        if (!v || isBlank(v)) return;
        QString host = v->url().host();
        if (host.startsWith("www.")) host.remove(0, 4);
        if (host.isEmpty()) return;
        QString t = host.section('.', 0, 0);
        if (!t.isEmpty()) t[0] = t.at(0).toUpper();
        const QString e = enc(t, v->url().toString());
        if (!favs.contains(e)) favs.append(e);
        saveFavs();
        refreshStartPages(false);
        QToolTip::showText(QCursor::pos(), "Pinned to Start Page");
    }

    // ---------------------------------------------------------------- tabs
    void openStartTab() {
        if (overviewActive) hideOverview();
        WebView *v = newTab(QUrl("about:blank"));
        v->setProperty("start", true);
        v->setHtml(startPageHtml(false), QUrl("about:blank"));
        urlBar->setFocus();
    }

    WebView *newTab(const QUrl &url) {
        auto *v = new WebView;
        v->opener = [this] { return newTab(QUrl("about:blank")); };
        v->command = [this](const QUrl &u) { handleCommand(u); };

        v->page()->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        v->page()->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
        v->page()->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
        v->page()->settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);
        v->page()->settings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, true);

        connect(v->page(), &QWebEnginePage::permissionRequested, this, [this](QWebEnginePermission perm) {
            handleWebEnginePermission(perm);
        });
#if QT_DEPRECATED_SINCE(6, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        connect(v->page(), &QWebEnginePage::featurePermissionRequested, this,
                [this, v](const QUrl &origin, QWebEnginePage::Feature feat) {
            handleFeaturePermission(v->page(), origin, feat);
        });
#pragma GCC diagnostic pop
#endif
        connect(v->page(), &QWebEnginePage::fullScreenRequested, this, [this](QWebEngineFullScreenRequest req) {
            if (req.toggleOn()) {
                req.accept();
                if (!isEffectivelyFullScreen()) {
                    savedBeforeVideoFullscreen = true;
                    toggleFullScreen();
                }
            } else {
                req.accept();
                if (savedBeforeVideoFullscreen) {
                    savedBeforeVideoFullscreen = false;
                    if (isEffectivelyFullScreen()) {
                        toggleFullScreen();
                    }
                }
            }
        });

        int idx = tabs->addTab(v, "Start Page");
        tabs->setCurrentIndex(idx);

        connect(v, &QWebEngineView::titleChanged, this, [this, v](const QString &t) {
            int i = tabs->indexOf(v);
            if (i >= 0) tabs->setTabText(i, t.left(28));
            if (v == view()) setWindowTitle(t);
            refreshSidebar();
        });
        connect(v, &QWebEngineView::iconChanged, this, [this, v](const QIcon &ic) {
            int i = tabs->indexOf(v);
            if (i >= 0) tabs->setTabIcon(i, ic);
            refreshSidebar();
        });
        connect(v, &QWebEngineView::urlChanged, this, [this, v](const QUrl &) {
            if (v == view()) {
                if (!urlBar->hasFocus()) showDisplay();
                updateLeadIcon();
                syncNav();
            }
        });
        connect(v, &QWebEngineView::loadStarted, this, [this, v] {
            if (v == view()) setProgress(6);
        });
        connect(v, &QWebEngineView::loadProgress, this, [this, v](int p) {
            if (v == view()) setProgress(p);
        });
        connect(v, &QWebEngineView::loadFinished, this, [this, v](bool) {
            if (v == view()) { setProgress(0); syncNav(); }
            QPointer<WebView> pv(v);
            QTimer::singleShot(700, this, [this, pv] {
                if (pv && pv.data() == view() && !overviewActive) captureThumb(pv.data());
            });
        });

        if (url.toString() != "about:blank") v->load(url);
        refreshSidebar();
        return v;
    }

    void closeTab(int i) {
        if (i < 0) return;
        if (tabs->count() <= 1) { close(); return; }
        QWidget *w = tabs->widget(i);
        thumbs.remove(w);
        tabs->removeTab(i);
        w->deleteLater();
        refreshSidebar();
        if (overviewActive) rebuildOverview();
    }

    // --------------------------------------------------------- address bar
    void updateLeadIcon() {
        auto *v = view();
        const bool blank = overviewActive || !v || isBlank(v);
        leadAct->setIcon(mkIcon(blank ? Ic::Search : Ic::PageMenu));
    }

    void showDisplay() {
        auto *v = view();
        if (!v || overviewActive) {
            urlBar->clear();
            urlBar->setAlignment(Qt::AlignHCenter);
            return;
        }
        QString t;
        if (!isBlank(v)) {
            const QUrl u = v->url();
            t = u.host().isEmpty() ? u.toString() : u.host();
        }
        if (t.startsWith("www.")) t.remove(0, 4);
        urlBar->setText(t);
        urlBar->setCursorPosition(0);
        urlBar->setAlignment(Qt::AlignHCenter);
    }

    void setProgress(int targetProgress) {
        if (!urlBar) return;
        if (targetProgress == 0) {
            if (progAnim && progAnim->state() == QAbstractAnimation::Running) progAnim->stop();
            currentDisplayProgress = 0;
            lastProgress = 0;
            urlBar->setStyleSheet(urlStyle(0));
            return;
        }

        if (!progAnim) {
            progAnim = new QVariantAnimation(this);
            connect(progAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &val) {
                currentDisplayProgress = val.toInt();
                lastProgress = currentDisplayProgress;
                urlBar->setStyleSheet(urlStyle(currentDisplayProgress));
            });
            connect(progAnim, &QVariantAnimation::finished, this, [this]() {
                if (currentDisplayProgress >= 100) {
                    QTimer::singleShot(250, this, [this]() {
                        currentDisplayProgress = 0;
                        lastProgress = 0;
                        urlBar->setStyleSheet(urlStyle(0));
                    });
                }
            });
        }

        if (targetProgress < currentDisplayProgress) {
            currentDisplayProgress = targetProgress;
        }

        progAnim->stop();
        progAnim->setDuration(240);
        progAnim->setEasingCurve(QEasingCurve::OutQuad);
        progAnim->setStartValue(currentDisplayProgress);
        progAnim->setEndValue(targetProgress);
        progAnim->start();
    }

    void syncNav() {
        auto *v = view();
        backBtn->setEnabled(v && v->history()->canGoBack());
        fwdBtn->setEnabled(v && v->history()->canGoForward());
        shareBtn->setEnabled(v && !isBlank(v));
    }

    void syncUi() {
        setProgress(0);
        if (!urlBar->hasFocus()) showDisplay();
        updateLeadIcon();
        syncNav();
        refreshSidebar();
        if (view()) setWindowTitle(titleOf(tabs->currentIndex()));
    }

    void navigate(const QString &text) {
        QString t = text.trimmed();
        if (t.isEmpty() || !view()) return;
        if (overviewActive) hideOverview();

        if (t == "safariqt://test" || t == "test" || t == "safariqt://permission-test") {
            openTestTab();
            return;
        }

        QUrl url;
        // Looks like a URL? Otherwise treat it as a search query.
        if (t.contains(' ') || (!t.contains('.') && !t.contains("://") && !t.startsWith("localhost"))) {
            url = QUrl("https://www.google.com/search?q=" + QUrl::toPercentEncoding(t));
        } else {
            url = QUrl::fromUserInput(t);
        }
        view()->load(url);
        urlBar->clearFocus();
        view()->setFocus();
    }

    // --------------------------------------------------------- permissions
    void queuePermission(const GenericPermission &p) {
        for (const auto &existing : pendingPermissions) {
            if (existing.origin == p.origin && existing.type == p.type) {
                if (p.resolver) p.resolver(false);
                return;
            }
        }
        pendingPermissions.append(p);
        showNextPermission();
    }

    void handleWebEnginePermission(QWebEnginePermission perm) {
        if (!perm.isValid()) return;
        GenericPermission gp;
        gp.origin = perm.origin();
        switch (perm.permissionType()) {
        case QWebEnginePermission::PermissionType::MediaAudioCapture:
            gp.type = GenericPermission::Type::Microphone; break;
        case QWebEnginePermission::PermissionType::MediaVideoCapture:
            gp.type = GenericPermission::Type::Camera; break;
        case QWebEnginePermission::PermissionType::MediaAudioVideoCapture:
            gp.type = GenericPermission::Type::CameraAndMicrophone; break;
        case QWebEnginePermission::PermissionType::Geolocation:
            gp.type = GenericPermission::Type::Location; break;
        case QWebEnginePermission::PermissionType::Notifications:
            gp.type = GenericPermission::Type::Notifications; break;
        case QWebEnginePermission::PermissionType::DesktopVideoCapture:
        case QWebEnginePermission::PermissionType::DesktopAudioVideoCapture:
            gp.type = GenericPermission::Type::ScreenShare; break;
        case QWebEnginePermission::PermissionType::ClipboardReadWrite:
            gp.type = GenericPermission::Type::Clipboard; break;
        default:
            gp.type = GenericPermission::Type::Other; break;
        }
        gp.resolver = [perm](bool granted) {
            if (granted) perm.grant();
            else perm.deny();
        };
        queuePermission(gp);
    }

#if QT_DEPRECATED_SINCE(6, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    void handleFeaturePermission(QWebEnginePage *page, const QUrl &origin, QWebEnginePage::Feature feature) {
        GenericPermission gp;
        gp.origin = origin;
        switch (feature) {
        case QWebEnginePage::MediaAudioCapture:
            gp.type = GenericPermission::Type::Microphone; break;
        case QWebEnginePage::MediaVideoCapture:
            gp.type = GenericPermission::Type::Camera; break;
        case QWebEnginePage::MediaAudioVideoCapture:
            gp.type = GenericPermission::Type::CameraAndMicrophone; break;
        case QWebEnginePage::Geolocation:
            gp.type = GenericPermission::Type::Location; break;
        case QWebEnginePage::Notifications:
            gp.type = GenericPermission::Type::Notifications; break;
        case QWebEnginePage::DesktopVideoCapture:
        case QWebEnginePage::DesktopAudioVideoCapture:
            gp.type = GenericPermission::Type::ScreenShare; break;
        default:
            gp.type = GenericPermission::Type::Other; break;
        }
        QPointer<QWebEnginePage> pSafe(page);
        gp.resolver = [pSafe, origin, feature](bool granted) {
            if (pSafe) {
                pSafe->setFeaturePermission(origin, feature,
                    granted ? QWebEnginePage::PermissionGrantedByUser
                            : QWebEnginePage::PermissionDeniedByUser);
            }
        };
        queuePermission(gp);
    }
#pragma GCC diagnostic pop
#endif

    void showNextPermission() {
        if (activeBanner || pendingPermissions.isEmpty()) return;
        GenericPermission perm = pendingPermissions.takeFirst();
        activeBanner = new PermissionBanner(centralWidget(), perm, [this](bool) {
            activeBanner = nullptr;
            showNextPermission();
        }, dark);

        int w = activeBanner->width();
        int x = (centralWidget()->width() - w) / 2;
        int y = 58;
        activeBanner->startSlideIn(x, y);
    }

    // ----------------------------------------------------------- downloads
    void handleDownloadRequested(QWebEngineDownloadRequest *dl) {
        QString name = dl->suggestedFileName();
        if (name.isEmpty()) name = dl->downloadFileName();
        if (name.isEmpty()) name = QFileInfo(QUrl(dl->url()).path()).fileName();
        if (name.isEmpty()) name = "download";

        // Sanitize file name
        name = QFileInfo(name).fileName();
        name.remove(QRegularExpression("[/\\\\?%*:|\"<>]"));
        if (name.isEmpty()) name = "download";

        const QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QDir().mkpath(defaultDir);

        // Auto-generate non-conflicting unique filename (e.g. file (1).zip)
        QString uniqueName = name;
        QFileInfo targetFi(defaultDir + "/" + uniqueName);
        if (targetFi.exists()) {
            QString base = targetFi.completeBaseName();
            QString ext = targetFi.suffix();
            if (!ext.isEmpty()) ext.prepend(".");
            int count = 1;
            while (QFileInfo::exists(defaultDir + "/" + QString("%1 (%2)%3").arg(base).arg(count).arg(ext))) {
                count++;
            }
            uniqueName = QString("%1 (%2)%3").arg(base).arg(count).arg(ext);
        }

        dl->setDownloadDirectory(defaultDir);
        dl->setDownloadFileName(uniqueName);
        dl->accept();

        // If the download was initiated in a popup/blank tab, do not close the tab immediately
        // because destroying the page cancels active downloads in Chromium. Only close once finished.
        if (auto *pg = dl->page()) {
            connect(dl, &QWebEngineDownloadRequest::stateChanged, this, [this, pg](QWebEngineDownloadRequest::DownloadState s) {
                if (s != QWebEngineDownloadRequest::DownloadInProgress) {
                    for (int i = 0; i < tabs->count(); ++i) {
                        if (viewAt(i)->page() == pg) {
                            if (isBlank(viewAt(i)) && tabs->count() > 1) {
                                closeTab(i);
                            }
                            break;
                        }
                    }
                }
            });
        }

        DownloadItem item;
        item.dl = dl;
        item.fileName = uniqueName;
        item.fullPath = defaultDir + "/" + uniqueName;
        item.receivedBytes = 0;
        item.totalBytes = dl->totalBytes();
        item.state = 0;
        downloadItems.prepend(item);

        dlBtn->show();
        animateDownloadButton();

        connect(dl, &QWebEngineDownloadRequest::receivedBytesChanged, this, [this, dl]() {
            for (auto &it : downloadItems) {
                if (it.dl == dl) {
                    it.receivedBytes = dl->receivedBytes();
                    it.totalBytes = dl->totalBytes();
                    break;
                }
            }
        });

        connect(dl, &QWebEngineDownloadRequest::stateChanged, this, [this, dl](QWebEngineDownloadRequest::DownloadState state) {
            bool hasActive = false;
            for (auto &it : downloadItems) {
                if (it.dl == dl) {
                    if (state == QWebEngineDownloadRequest::DownloadCompleted) {
                        it.state = 1;
                        it.receivedBytes = it.totalBytes;
                    } else if (state == QWebEngineDownloadRequest::DownloadCancelled) {
                        it.state = 2;
                    } else if (state == QWebEngineDownloadRequest::DownloadInterrupted) {
                        it.state = 3;
                    }
                }
                if (it.state == 0) hasActive = true;
            }
            if (!hasActive) {
                stopDownloadAnimation(state == QWebEngineDownloadRequest::DownloadCompleted);
            }
        });
    }

    void showDownloadsPanel() {
        auto *p = new DownloadsPanel(this, downloadItems,
            [this]() {
                downloadItems.clear();
                if (downloadItems.isEmpty()) {
                    dlBtn->hide();
                }
            },
            [this](QWebEngineDownloadRequest *dl) {
                if (dl) dl->cancel();
            });
        p->adjustSize();
        const QPoint g = dlBtn->mapToGlobal(QPoint(dlBtn->width() / 2, dlBtn->height() + 6));
        p->move(g.x() - p->width() / 2, g.y());
        p->showAnimated();
    }

    void animateDownloadButton() {
        if (!dlBtnAnim) {
            dlBtnAnim = new QVariantAnimation(this);
            dlBtnAnim->setDuration(600);
            dlBtnAnim->setStartValue(0.0);
            dlBtnAnim->setEndValue(1.0);
            dlBtnAnim->setLoopCount(-1);
            connect(dlBtnAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &v) {
                qreal phase = v.toReal();
                QColor pulseColor = dark ? QColor("#4aa3ff") : QColor("#0a84ff");
                pulseColor.setAlphaF(0.4 + 0.6 * std::sin(phase * kPi));
                dlBtn->setIcon(mkIconColor(Ic::Download, pulseColor));
            });
        }
        if (dlBtnAnim->state() != QAbstractAnimation::Running) {
            dlBtnAnim->start();
        }
    }

    void stopDownloadAnimation(bool completed) {
        if (dlBtnAnim) {
            dlBtnAnim->stop();
        }
        dlBtn->setIcon(mkIcon(Ic::Download));
        if (completed) {
            dlBtn->setIcon(mkIconColor(Ic::Download, QColor("#34c759")));
            QTimer::singleShot(2500, this, [this]() {
                dlBtn->setIcon(mkIcon(Ic::Download));
            });
        }
    }
};

int main(int argc, char *argv[]) {
    // Must be set before QApplication is created.
    QByteArray flags = qgetenv("QTWEBENGINE_CHROMIUM_FLAGS");
    flags += " --enable-gpu-rasterization --enable-zero-copy --ignore-gpu-blocklist"
             " --enable-smooth-scrolling --use-fake-device-for-media-stream"
             " --enable-features=WebRTCPipeWireCapturer,AudioServiceOutOfProcess";
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", flags);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);
    Browser b;
    b.show();
    return app.exec();
}