#include "EditTaskDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

EditTaskDialog::EditTaskDialog(Task task, QWidget* parent) : QDialog(parent), originalTask(task){
    setWindowTitle("Edit Task");
    resize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    titleInput = new QLineEdit(task.getTitle());
    descriptionInput = new QTextEdit(task.getDescription());

    dueDateInput = new QDateEdit(task.getDueDate());
    dueDateInput->setCalendarPopup(true);

    priorityInput = new QComboBox();
    priorityInput->addItems({"Low", "Medium", "High"});
    priorityInput->setCurrentIndex(task.getPriority());

    categoryInput = new QLineEdit(task.getCategory());

    QHBoxLayout* btnLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save");
    cancelButton = new QPushButton("Cancel");

    btnLayout->addWidget(saveButton);
    btnLayout->addWidget(cancelButton);

    mainLayout->addWidget(new QLabel("Title:"));
    mainLayout->addWidget(titleInput);
    mainLayout->addWidget(new QLabel("Description:"));
    mainLayout->addWidget(descriptionInput);
    mainLayout->addWidget(new QLabel("Due Date:"));
    mainLayout->addWidget(dueDateInput);
    mainLayout->addWidget(new QLabel("Priority:"));
    mainLayout->addWidget(priorityInput);
    mainLayout->addWidget(new QLabel("Category:"));
    mainLayout->addWidget(categoryInput);
    mainLayout->addLayout(btnLayout);

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
}

Task EditTaskDialog::getUpdateTask() const{
    return Task{
        titleInput->text(),
        descriptionInput->toPlainText(),
        dueDateInput->date(),
        categoryInput->text(),
        priorityInput->currentIndex(),
        originalTask.isCompleted()
    };
}

void EditTaskDialog::accept() {
    if (titleInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Task title cannot be empty!");
        return;
    }
    if (dueDateInput->date() < QDate::currentDate()) {
        QMessageBox::warning(this, "Input Error", "Due date cannot be in the past!");
        return;
    }
    QDialog::accept();
}