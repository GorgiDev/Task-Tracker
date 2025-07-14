#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "Task.h"

class EditTaskDialog : public QDialog{
    Q_OBJECT
    public:
        EditTaskDialog(Task task, QWidget* parent = nullptr);
        Task getUpdateTask() const;
    private:
        Task originalTask;

        QLineEdit* titleInput;
        QTextEdit* descriptionInput;
        QDateEdit* dueDateInput;
        QComboBox* priorityInput;
        QLineEdit* categoryInput;

        QPushButton* saveButton;
        QPushButton* cancelButton;
    protected:
        void accept() override;
};

#endif