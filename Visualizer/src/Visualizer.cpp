#include <QFileDialog>
#include <QGridLayout>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"
#include "TriangulationSetOperations.h"
#include "Triangulation.h"


Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(loadFile1, &QPushButton::clicked, this, &Visualizer::onLoadFileClick1);
    connect(join, &QPushButton::clicked, this, &Visualizer::onJoinClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
}

Visualizer::~Visualizer()
{

}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File1", this);
    loadFile1 = new QPushButton("Load File2", this);
    join = new QPushButton("Join", this);
    exportFile = new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetInput1 = new OpenGlWidget1(this);
    openglWidgetOutput = new OpenGlWidget(this);
    //graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetInput1 , openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(loadFile1, 0, 1);
    layout->addWidget(join, 0, 2);
    layout->addWidget(exportFile, 0, 3);
    layout->addWidget(openglWidgetInput, 1, 0, 1, 2);
    layout->addWidget(openglWidgetInput1, 1, 2, 1, 2);
    layout->addWidget(openglWidgetOutput, 2, 0, 1, 4);
    


    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}

void  Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
        {
            STLReader reader;
            reader.read(inputFilePath.toStdString(), triangulation);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            OBJReader reader;
            reader.read(inputFilePath.toStdString(), triangulation);
        }
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        openglWidgetInput->setData(data);
    }
}

void  Visualizer::onLoadFileClick1()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
        {
            STLReader reader;
            reader.read(inputFilePath.toStdString(), triangulation1);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            OBJReader reader;
            reader.read(inputFilePath.toStdString(), triangulation1);
        }
        OpenGlWidget1::Data data1 = convertTrianglulationToGraphicsObject1(triangulation1);
        openglWidgetInput1->setData1(data1);
    }
}

//void Visualizer::onTranslateClick()
//{
//    QFileDialog dialog(this);
//
//    dialog.setFileMode(QFileDialog::Directory);
//
//    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//        "/home",
//        QFileDialog::ShowDirsOnly
//        | QFileDialog::DontResolveSymlinks);
//
//
//    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
//    {
//        QString exportFileName = dir + "/output.obj";
//        Geometry::Matrix4x4 mat;
//        Transformation::Transformation t;
//        /*outputTriangulation = t.scaling(triangulation, mat);*/
//        outputTriangulation = t.translation(triangulation, mat, 8.21017 - 4.51358 - 49.1144);
//        /*outputTriangulation = t.rotationX(triangulation, mat);*/
//        /*outputTriangulation = t.rotationX(triangulation, mat);*/
//        ObjWriter writer;
//        writer.Write(exportFileName.toStdString(), outputTriangulation);
//
//        // reload file to check and load in output renderer
//        OBJReader reader;
//        reader.read(exportFileName.toStdString(), outputTriangulation);
//
//        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
//        openglWidgetOutput->setData(data);
//
//    }
//    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
//    {
//        QString exportFileName = dir + "/output.stl";
//        Geometry::Matrix4x4 mat;
//        Transformation::Transformation t;
//        outputTriangulation = t.translation(triangulation, mat, 8.21017 - 4.51358 - 49.1144);
//        STLWriter writer;
//        writer.Write(exportFileName.toStdString(), outputTriangulation);
//
//        // reload file to check and load in output renderer
//        STLReader reader;
//        reader.read(exportFileName.toStdString(), outputTriangulation);
//
//        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
//        openglWidgetOutput->setData(data);
//    }
//
//}

void Visualizer::onJoinClick()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.obj";
        Geometry::Matrix4x4 mat;
        Transformation::Transformation t;

        // Write the combined triangulation data to the output file
        ObjWriter writer;
        writer.Write(exportFileName.toStdString(), outputTriangulation);

        // Reload file to check and load in output renderer
        OBJReader reader;
        reader.read(exportFileName.toStdString(), outputTriangulation);

        // Convert triangulation to OpenGL data for rendering
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        //triangulation1 = t.translation(triangulation, mat, 10);
        OpenGlWidget::Data data2 = convertTrianglulationToGraphicsObject(triangulation1);
        for (auto i : data.normals)
        {
            data2.normals.append(i);
        }
        for (auto j : data.vertices)
        {
            data2.vertices.append(j);
        }
        openglWidgetOutput->setData(data2);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.stl";
        Geometry::Matrix4x4 mat;
        Transformation::Transformation t;

        // Write the combined triangulation data to the output file
        STLWriter writer;
        writer.Write(exportFileName.toStdString(), outputTriangulation);

        // Reload file to check and load in output renderer
        STLReader reader;
        reader.read(exportFileName.toStdString(), outputTriangulation);

        // Convert triangulation to OpenGL data for rendering
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        OpenGlWidget::Data data1 = convertTrianglulationToGraphicsObject(triangulation1);
        OpenGlWidget::Data data2;
        
        for (auto i : data.normals)
        {
            data2.normals.append(i);
        }
        for (auto j : data.vertices)
        {
            data2.vertices.append(j);
        }
        openglWidgetOutput->setData(data2);
    }
}



//#include "TriangulationSetOperations.h"
//
//void Visualizer::onJoinClick()
//{
//    QFileDialog dialog(this);
//    dialog.setFileMode(QFileDialog::Directory);
//
//    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//        "/home",
//        QFileDialog::ShowDirsOnly
//        | QFileDialog::DontResolveSymlinks);
//
//    // Perform the join side by side operation on the two triangulations
//    Geometry::Triangulation resultTriangulation = Geometry::TriangulationSetOperations::joinSideBySide(triangulation, triangulation1);
//
//    // Check the file format and export the result accordingly
//    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
//    {
//        QString exportFileName = dir + "/output.obj";  // Set the export path for .obj
//        ObjWriter writer;
//        writer.Write(exportFileName.toStdString(), resultTriangulation);
//
//        // Reload the exported .obj file
//        OBJReader reader;
//        reader.read(exportFileName.toStdString(), resultTriangulation);
//
//        // Convert triangulation to graphics data and send to OpenGL widget
//        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(resultTriangulation);
//        openglWidgetOutput->setData(data);
//
//    }
//    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
//    {
//        QString exportFileName = dir + "/output.stl";  // Set the export path for .stl
//        STLWriter writer;
//        writer.Write(exportFileName.toStdString(), resultTriangulation);
//
//        // Reload the exported .stl file
//        STLReader reader;
//        reader.read(exportFileName.toStdString(), resultTriangulation);
//
//        // Convert triangulation to graphics data and send to OpenGL widget
//        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(resultTriangulation);
//        openglWidgetOutput->setData(data);
//    }
//}




void Visualizer::onExportClick()
{
    QFileDialog dialog(this);

    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        ObjWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation);
    }
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
    for each (Triangle triangle in inTriangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }

        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }
    }

    return data;
}

OpenGlWidget1::Data Visualizer::convertTrianglulationToGraphicsObject1(const Triangulation& inTriangulation)
{
    OpenGlWidget1::Data data;
    for each (Triangle triangle in inTriangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }

        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }
    }

    return data;
}



//#include <QFileDialog>
//#include <QGridLayout>
//#include "Visualizer.h"
//#include "STLReader.h"
//#include "OBJReader.h"
//#include "OBJWriter.h"
//#include "STLWriter.h"
//#include "DataWriter.h"
//#include "Triangle.h"
//
//Visualizer::Visualizer(QWidget* parent)
//    : QMainWindow(parent)
//{
//    setupUi();
//
//    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
//    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
//    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
//}
//
//Visualizer::~Visualizer()
//{
//    // Cleanup if necessary
//}
//
//void Visualizer::setupUi()
//{
//    loadFile = new QPushButton("Load File", this);
//    translate = new QPushButton("Translate", this);
//    exportFile = new QPushButton("Export", this);
//    openglWidgetInput = new OpenGlWidget(this);
//    openglWidgetOutput = new OpenGlWidget(this);
//    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);
//
//    QGridLayout* layout = new QGridLayout(this);
//
//    layout->addWidget(loadFile, 0, 0);
//    layout->addWidget(translate, 0, 1);
//    layout->addWidget(openglWidgetInput, 1, 0);
//    layout->addWidget(openglWidgetOutput, 1, 1, 1, 2);
//    layout->addWidget(exportFile, 0, 2);
//
//    QWidget* centralWidget = new QWidget(this);
//    setCentralWidget(centralWidget);
//    centralWidget->setLayout(layout);
//}
//
//void Visualizer::onLoadFileClick()
//{
//    QString fileName = QFileDialog::getOpenFileName(this,
//        tr("Open File"), "", tr("files (*.stl *.obj)"));
//
//    if (!fileName.isEmpty())
//    {
//        inputFilePath = fileName;
//        loadAndSetData(inputFilePath, openglWidgetInput);
//    }
//}
//
//void Visualizer::onTranslateClick()
//{
//    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//        "/home",
//        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//
//    if (!dir.isEmpty() && !inputFilePath.isEmpty())
//    {
//        QString exportFileName = dir + "/output" + getOppositeExtension(inputFilePath);
//        writeFile(exportFileName, triangulation);
//
//        // Reload file to check and load in output renderer
//        loadAndSetData(exportFileName, openglWidgetOutput);
//    }
//}
//
//void Visualizer::onExportClick()
//{
//    if (!inputFilePath.isEmpty())
//    {
//        QString oppositeExt = getOppositeExtension(inputFilePath);
//        QString fileName = QFileDialog::getSaveFileName(this,
//            tr("Save File"), "", tr("files (*") + oppositeExt + ")");
//
//        if (!fileName.isEmpty())
//        {
//            writeFile(fileName, outputTriangulation);
//        }
//    }
//}
//
//void Visualizer::loadAndSetData(const QString& filePath, OpenGlWidget* widget)
//{
//    Triangulation localTriangulation;
//    if (filePath.endsWith(".stl", Qt::CaseInsensitive))
//    {
//        STLReader reader;
//        reader.read(filePath.toStdString(), localTriangulation);
//    }
//    else if (filePath.endsWith(".obj", Qt::CaseInsensitive))
//    {
//        OBJReader reader;
//        reader.read(filePath.toStdString(), localTriangulation);
//    }
//
//    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(localTriangulation);
//    widget->setData(data);
//
//    // Update the appropriate triangulation
//    if (widget == openglWidgetInput)
//        triangulation = localTriangulation;
//    else if (widget == openglWidgetOutput)
//        outputTriangulation = localTriangulation;
//}
//
//QString Visualizer::getOppositeExtension(const QString& filePath)
//{
//    return filePath.endsWith(".stl", Qt::CaseInsensitive) ? ".obj" : ".stl";
//}
//
//void Visualizer::writeFile(const QString& fileName, const Triangulation& triangulationData)
//{
//    if (fileName.endsWith(".obj", Qt::CaseInsensitive))
//    {
//        ObjWriter writer;
//        writer.Write(fileName.toStdString(), triangulationData);
//    }
//    else if (fileName.endsWith(".stl", Qt::CaseInsensitive))
//    {
//        STLWriter writer;
//        writer.Write(fileName.toStdString(), triangulationData);
//    }
//}
//
//OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
//{
//    OpenGlWidget::Data data;
//    for each (Triangle triangle in inTriangulation.Triangles)
//    {
//        for each (Point point in triangle.Points())
//        {
//            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
//            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
//            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
//        }
//
//        Point normal = triangle.Normal();
//
//        for (size_t i = 0; i < 3; i++)
//        {
//            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
//            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
//            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
//        }
//    }
//
//    return data;
//}


