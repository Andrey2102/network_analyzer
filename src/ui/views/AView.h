#ifndef AVIEW_H
#define AVIEW_H

#include <QWidget>

class AView : public QWidget {
    Q_OBJECT

public:
    explicit AView(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~AView() {}

    // Pure virtual function to be implemented by derived classes
    virtual void setupUI() = 0;
};

#endif // AVIEW_H