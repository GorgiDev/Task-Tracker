#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QEvent>
#include <QEnterEvent>
#include <QCheckBox>
#include "Task.h"

class TaskWidget : public QWidget{
    Q_OBJECT
    public:
        TaskWidget(Task task, QWidget* parent = nullptr);
        Task getTask() const;
    signals:
        void editRequested(const Task& task);
        void deleteRequested(const Task& task);
        void toggleComplete(const Task& task);
    private slots:
        void handleEdit();
        void handleDelete();
        void handleToggleComplete();
    protected:
        void enterEvent (QEnterEvent* event) override;
        void leaveEvent (QEvent* event) override;
    private:
        Task task;
        QLabel* titleLabel;
        QLabel* dueDateLabel;
        QLabel* priorityLabel;
        QCheckBox* completeCheckbox;
        QPushButton* editButton;
        QPushButton* deleteButton;
};

#endif