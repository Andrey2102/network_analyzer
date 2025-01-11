#ifndef ABSTRACT_VIEW_H
#define ABSTRACT_VIEW_H

#include <QWidget>

class AbstractView : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractView(QWidget *parent = nullptr);
    virtual ~AbstractView();

    // Pure virtual function to be implemented by derived classes
    virtual void setupUI() = 0;
};

#endif // ABSTRACT_VIEW_H