#include "TaskWidget.h"

#include "TaskWidget.h"

TaskWidget::TaskWidget(Task task, QWidget* parent)
    : QWidget(parent), task(task) {

    QString prioText;
    switch (task.getPriority()){
        case 0: prioText = "Low"; break;
        case 1: prioText = "Medium"; break;
        case 2: prioText = "High"; break;
        case 3: prioText = "Unknown"; break;
    }

    titleLabel = new QLabel(task.getTitle());
    dueDateLabel = new QLabel("Due: " + task.getDueDate().toString("dd/MM/yy"));
    priorityLabel = new QLabel("Priority: " + prioText);

    titleLabel->setObjectName("titleLabel");
    dueDateLabel->setObjectName("dueDateLabel");
    priorityLabel->setObjectName("priorityLabel");

    completeCheckbox = new QCheckBox();
    completeCheckbox->setChecked(task.isCompleted());
    completeCheckbox->setToolTip("Mark Complete");

    editButton = new QPushButton("✎");
    editButton->setToolTip("Edit Task");
    editButton->setObjectName("editButton");

    deleteButton = new QPushButton("✗");
    deleteButton->setToolTip("Delete Task");
    deleteButton->setObjectName("deleteButton");

    completeCheckbox->setVisible(false);
    editButton->setVisible(false);
    deleteButton->setVisible(false);

    // Container widget with layout
    QWidget* container = new QWidget(this);
    container->setObjectName("container");
    QHBoxLayout* containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(8, 8, 8, 8);

    containerLayout->addWidget(titleLabel);
    containerLayout->addWidget(dueDateLabel);
    containerLayout->addWidget(priorityLabel);
    containerLayout->addStretch();
    containerLayout->addWidget(completeCheckbox);
    containerLayout->addWidget(editButton);
    containerLayout->addWidget(deleteButton);

    // Main layout for TaskWidget that contains the container widget
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(container);

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    connect(editButton, &QPushButton::clicked, this, &TaskWidget::handleEdit);
    connect(deleteButton, &QPushButton::clicked, this, &TaskWidget::handleDelete);
    connect(completeCheckbox, &QCheckBox::toggled, this, [this](bool checked) {
        this->task.setCompleted(checked);
        titleLabel->setStyleSheet(checked ? "text-decoration: line-through; color: #9E9E9E" : "");
        emit toggleComplete(this->task);
    });

    setMouseTracking(true);
    installEventFilter(this);
}

void TaskWidget::handleToggleComplete() {
    task.setCompleted(!task.isCompleted());
    completeCheckbox->setText(task.isCompleted() ? "Undo" : "Complete");
    titleLabel->setStyleSheet(task.isCompleted()
        ? "text-decoration: line-through; color: #9E9E9E"
        : "");
    emit toggleComplete(task);
}

void TaskWidget::handleEdit() {
    emit editRequested(task);
}

void TaskWidget::handleDelete() {
    emit deleteRequested(task);
}

void TaskWidget::enterEvent(QEnterEvent* event){
    completeCheckbox->setVisible(true);
    editButton->setVisible(true);
    deleteButton->setVisible(true);
    QWidget::enterEvent(event);
}


void TaskWidget::leaveEvent(QEvent* event){
    completeCheckbox->setVisible(false);
    editButton->setVisible(false);
    deleteButton->setVisible(false);
    QWidget::leaveEvent(event);
}


Task TaskWidget::getTask() const {
    return task;
}
