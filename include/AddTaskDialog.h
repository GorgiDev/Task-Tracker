#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include "Task.h"

class AddTaskDialog : public QDialog{
    Q_OBJECT;
    public:
        explicit AddTaskDialog(QWidget* parent = nullptr);
        Task getNewTask() const;
    private:
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