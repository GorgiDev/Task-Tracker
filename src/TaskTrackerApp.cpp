#include "TaskTrackerApp.h"
#include "EditTaskDialog.h"
#include "AddTaskDialog.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QCheckBox>
#include <QMenu>
#include <QMenuBar>
#include <QStackedLayout>

TaskTrackerApp::TaskTrackerApp(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Task Tracker");
    setWindowIcon(QIcon(":/assets/icons/app_icon.ico"));
    resize(800, 600);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout");

    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");

    QMenu* fileMenu = menuBar->addMenu("File");
    fileMenu->addAction("Import Tasks", this, &TaskTrackerApp::importTasks);
    fileMenu->addAction("Export Tasks", this, &TaskTrackerApp::exportTasks);

    mainLayout->setMenuBar(menuBar);

    QHBoxLayout* controlsLayout = new QHBoxLayout();
    controlsLayout->setObjectName("controlsLayout");

    filterComboBox = new QComboBox();
    filterComboBox->addItems({"All", "Completed", "Incomplete"});
    filterComboBox->setObjectName("filterComboBox");

    sortComboBox = new QComboBox();
    sortComboBox->addItems({"Due Date", "Priority", "Title"});
    sortComboBox->setObjectName("sortComboBox");

    QPushButton* exportButton = new QPushButton("Export Tasks");
    exportButton->setObjectName("exportButton");

    QPushButton* importButton = new QPushButton("Import Tasks");
    importButton->setObjectName("importButton");

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search by title...");
    searchInput->setObjectName("searchInput");

    categoryFilterComboBox = new QComboBox();
    categoryFilterComboBox->addItem("All Categories");
    categoryFilterComboBox->setObjectName("categoryFilterComboBox");

    controlsLayout->addWidget(new QLabel("Filter: "));
    controlsLayout->addWidget(filterComboBox);
    controlsLayout->addSpacing(20);

    controlsLayout->addWidget(new QLabel("Sort by:"));
    controlsLayout->addWidget(sortComboBox);

    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(new QLabel("Search: "));
    controlsLayout->addWidget(searchInput);

    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(exportButton);
    controlsLayout->addWidget(importButton);

    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(new QLabel("Category: "));
    controlsLayout->addWidget(categoryFilterComboBox);

    connect(filterComboBox, &QComboBox::currentTextChanged, this, TaskTrackerApp::refreshTaskList);
    connect(sortComboBox, &QComboBox::currentTextChanged, this, &TaskTrackerApp::refreshTaskList);
    connect(searchInput, &QLineEdit::textChanged, this, &TaskTrackerApp::refreshTaskList);
    connect(exportButton, &QPushButton::clicked, this, &TaskTrackerApp::exportTasks);
    connect(importButton, &QPushButton::clicked, this, &TaskTrackerApp::importTasks);
    connect(categoryFilterComboBox, &QComboBox::currentTextChanged, this, &TaskTrackerApp::refreshTaskList);

    mainLayout->addLayout(controlsLayout);

    taskListLayout = new QVBoxLayout();
    taskListLayout->setAlignment(Qt::AlignTop);
    taskListLayout->setObjectName("taskListLayout");

    taskListContainer = new QWidget();
    taskListContainer->setLayout(taskListLayout);
    taskListContainer->setObjectName("taskListContainer");

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(taskListContainer);
    scrollArea->setObjectName("scrollArea");

    // Container for scroll area and floating button
    QWidget* scrollContainer = new QWidget(this);
    scrollContainer->setObjectName("scrollContainer");

    QGridLayout* overlayLayout = new QGridLayout(scrollContainer);
    overlayLayout->setContentsMargins(0, 0, 0, 0);
    overlayLayout->setSpacing(0);

    QWidget* contentHolder = new QWidget(this);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentHolder);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(scrollArea);

    overlayLayout->addWidget(contentHolder, 0, 0);

    // Floating Add Button
    QPushButton* floatingAddButton = new QPushButton("+", scrollContainer);
    floatingAddButton->setFixedSize(48, 48);
    floatingAddButton->setObjectName("addButton");

    QWidget* buttonWrapper = new QWidget(scrollContainer);
    buttonWrapper->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QHBoxLayout* wrapperLayout = new QHBoxLayout(buttonWrapper);
    wrapperLayout->addStretch();
    wrapperLayout->addWidget(floatingAddButton);
    wrapperLayout->setContentsMargins(0, 0, 16, 16);
    wrapperLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);

    overlayLayout->addWidget(buttonWrapper, 0, 0, Qt::AlignBottom | Qt::AlignRight);

    mainLayout->addWidget(scrollContainer);

    connect(floatingAddButton, &QPushButton::clicked, this, [this]() {
        AddTaskDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            Task newTask = dialog.getNewTask();
            tasks.append(newTask);

            if (!newTask.getCategory().isEmpty() &&
                categoryFilterComboBox->findText(newTask.getCategory()) == -1) {
                    categoryFilterComboBox->addItem(newTask.getCategory());
                }

            saveTasks();
            refreshTaskList();
        }
    });

    reminderTimer = new QTimer(this);
    reminderTimer->setInterval(60000);
    connect(reminderTimer, &QTimer::timeout, this, &TaskTrackerApp::checkReminders);
    reminderTimer->start();

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/assets/icons/app_icon.ico"));
    trayIcon->show();

    loadTasks();
    refreshTaskList();
}

QList<Task> TaskTrackerApp::getFilteredTasks() const {
    QList<Task> filtered;
    QString filter = filterComboBox->currentText();
    QString searchTerm = searchInput->text().trimmed().toLower();
    QString selectedCategory = categoryFilterComboBox->currentText();

    for (const Task& t : tasks){
        bool passesFilter =
            (filter == "All") ||
            (filter == "Completed" && t.isCompleted()) ||
            (filter == "Incomplete" && !t.isCompleted());

        bool matchesSearch = t.getTitle().toLower().contains(searchTerm);

        bool matchesCategory = 
            (selectedCategory == "All Categories") || 
            (t.getCategory() == selectedCategory);

        if (passesFilter && matchesSearch && matchesCategory){
            filtered.append(t);
        }
    }

    return filtered;
}

void TaskTrackerApp::addTask(){
    QString title = titleInput->text().trimmed();
    if (title.isEmpty()){
        QMessageBox::warning(this, "Input Error", "Task title cannot be empty!");
        return;
    }

    QDate due = dueDateInput->date();
    int prio = priorityInput->currentIndex();
    QString category = categoryInput->text().trimmed();

    Task task(title, "", due, category, prio, false);
    tasks.append(task);

    if (!category.isEmpty() && categoryFilterComboBox->findText(category) == -1)
        categoryFilterComboBox->addItem(category);

    titleInput->clear();
    categoryInput->clear();
    
    refreshTaskList();
    saveTasks();
}

void TaskTrackerApp::refreshTaskList(){
    QLayoutItem* child;
    while ((child = taskListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    QList<Task> filtered = getFilteredTasks();
    sortTasks(filtered);

    for (int i = 0; i < filtered.size(); ++i){
        const Task& t = filtered[i];
        TaskWidget* widget = new TaskWidget(t, this);

        widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QColor bgColor = (i % 2 == 0)
            ? QColor("#626262") : QColor("#4b4b4b");
        widget->setStyleSheet(QString("background-color: %1; border-radius: 6px; font-size: 16px; padding: 10px;").arg(bgColor.name()));

        connect(widget, &TaskWidget::editRequested, this, &TaskTrackerApp::editTask);
        connect(widget, &TaskWidget::deleteRequested, this, &TaskTrackerApp::removeTask);
        connect(widget, &TaskWidget::toggleComplete, this, [this](const Task& toggledTask){
            for (Task& t : tasks){
                if (t.getId() == toggledTask.getId()){
                    t.setCompleted(toggledTask.isCompleted());
                    break;
                }
            }
            saveTasks();
            refreshTaskList();
        });

        taskListLayout->addWidget(widget);
    }
}

void TaskTrackerApp::removeTask(const Task& task){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete", 
                                  QString("Are you sure you want to delete task \"%1\"?").arg(task.getTitle()),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        for(int i = 0; i < tasks.size(); ++i){
            if (tasks[i].getId() == task.getId()){
                tasks.remove(i);
                break;
            }
        }
        saveTasks();
        refreshTaskList();
    }
}

void TaskTrackerApp::toggleCompleteTask(const Task& task){
    for (Task& t : tasks){
        if (t.getId() == task.getId()){
            t.setCompleted(!t.isCompleted());
            break;
        }
    }
    refreshTaskList();
    saveTasks();
}

void TaskTrackerApp::editTask(const Task& task){
    EditTaskDialog dialog(task, this);
    if (dialog.exec() == QDialog::Accepted){
        Task updated = dialog.getUpdateTask();

        updated.setNotified(false);

        for (int i = 0; i < tasks.size(); ++i){
            if (tasks[i].getId() == task.getId()){
                tasks[i] = updated;
                break;
            }
        }
    
        saveTasks();
        refreshTaskList();
    }
}

void TaskTrackerApp::saveTasks() const{
    QDir dir("data");
    if (!dir.exists()){
        dir.mkpath(".");
    }
    
    QJsonArray array;
    for (const Task& t : tasks)
        array.append(t.toJson());
    
    QJsonDocument doc(array);

    QFile file("data/tasks.json");
    if (file.open(QIODevice::WriteOnly)){
        file.write(doc.toJson());
        file.close();
    }

    QSet<QString> categorySet;
    for (const Task& t : tasks) {
        if (!t.getCategory().isEmpty())
            categorySet.insert(t.getCategory());
    }
    QJsonArray catArray;
    for (const QString& cat : categorySet)
        catArray.append(cat);

    QJsonDocument catDoc(catArray);
    QFile catFile("data/categories.json");
    if (catFile.open(QIODevice::WriteOnly)) {
        catFile.write(catDoc.toJson());
        catFile.close();
    }
}

void TaskTrackerApp::loadTasks(){
    QFile file("data/tasks.json");
    if (!file.exists()){
        return;
    }

    if (file.open(QIODevice::ReadOnly)){
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray array = doc.array();
        tasks.clear();
        for(const QJsonValue& v : array)
            tasks.append(Task::fromJson(v.toObject()));
        refreshTaskList();
    }
    QFile catFile("data/categories.json");
    if (catFile.exists() && catFile.open(QIODevice::ReadOnly)) {
        QJsonDocument catDoc = QJsonDocument::fromJson(catFile.readAll());
        QJsonArray catArray = catDoc.array();
        categoryFilterComboBox->clear();
        categoryFilterComboBox->addItem("All Categories");
        for (const QJsonValue& val : catArray) {
            QString cat = val.toString();
            if (!cat.isEmpty())
                categoryFilterComboBox->addItem(cat);
        }
        catFile.close();
    }
}

void TaskTrackerApp::sortTasks(QList<Task>& list) const{
    QString sortKey = sortComboBox->currentText();

    if (sortKey == "Due Date") {
        std::sort(list.begin(), list.end(), [](const Task& a, const Task& b) {
            return a.getDueDate() < b.getDueDate();
        });
    } else if (sortKey == "Priority") {
        std::sort(list.begin(), list.end(), [](const Task& a, const Task& b) {
            return a.getPriority() > b.getPriority();  // higher priority first
         });
    } else if (sortKey == "Title") {
        std::sort(list.begin(), list.end(), [](const Task& a, const Task& b) {
            return a.getTitle().toLower() < b.getTitle().toLower();
        });
    }
}

void TaskTrackerApp::checkReminders(){
    QDateTime now = QDateTime::currentDateTime();

    for (Task& task : tasks){
        if (!task.isCompleted() && !task.isNotified()){
            QDateTime dueDate(task.getDueDate(), QTime(0, 0));
            qint64 secsToDue = now.secsTo(dueDate);

            if (secsToDue > 0 && secsToDue <= 900){
                trayIcon->showMessage("Task Reminder", QString("Task '%1' is due soon!").arg(task.getTitle()), QSystemTrayIcon::Information, 5000);
                task.setNotified(true);
                saveTasks();
            }
        }
    }
}

void TaskTrackerApp::exportTasks(){
    QString fileName = QFileDialog::getSaveFileName(this, "Export Tasks", "", "CSV Files (*.csv);;Text Files(*.txt)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Export Error", "Failed to open file for writing.");
            return;
        }
    
    QTextStream out(&file);
    out << "ID,Title,Description,Due Date,Category,Priority,Completed\n";
    for (const Task& task : tasks) {
        out << "\"" << task.getId() << "\","
            << "\"" << task.getTitle() << "\","
            << "\"" << task.getDescription() << "\","
            << task.getDueDate().toString(Qt::ISODate) << ","
            << "\"" << task.getCategory() << "\","
            << task.getPriority() << ","
            << (task.isCompleted() ? "Yes" : "No") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Export Complete", "Tasks Exported successfully");
}

void TaskTrackerApp::importTasks(){
    QString fileName = QFileDialog::getOpenFileName(this, "Import Tasks", "", "CSV Files (*.csv);;Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Import Error", "Failed to open file.");
        return;
    }

    QTextStream in(&file);
    QString headerLine = in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() < 7) continue;

        QString id = fields[0].remove('"').trimmed();
        QString title = fields[1].remove('"').trimmed();
        QString description = fields[2].remove('"').trimmed();
        QDate dueDate = QDate::fromString(fields[3].trimmed(), Qt::ISODate);
        QString category = fields[4].remove('"').trimmed();
        int priority = fields[5].trimmed().toInt();
        bool completed = fields[6].trimmed().toLower() == "yes";

        tasks.append(Task(title, description, dueDate, category, priority, completed, id));
    }

    file.close();
    saveTasks();
    refreshTaskList();

    for (const Task& t : tasks){
        if (t.getCategory().isEmpty() && categoryFilterComboBox->findText(t.getCategory()) == -1){
            categoryFilterComboBox->addItem(t.getCategory());
        }
    }

    QMessageBox::information(this, "Import Complete", "Tasks imported successfully");
}