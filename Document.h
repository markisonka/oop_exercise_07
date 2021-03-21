#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <memory>

#include "FiguresAndFactories/Figure.h"
#include "FiguresAndFactories/Circle/Circle.h"
#include "FiguresAndFactories/Polygon/Trapeze/Trapeze.h"
#include "FiguresAndFactories/Polygon/RandomPolygon/RandomPolygon.h"
#include "FiguresAndFactories/Polygon/Rhombus/Rhombus.h"


class Document;
class Command;

class Command {
    friend Document;
public:
    Command(Document& doc);
    virtual ~Command() = default;

    virtual void Undo() = 0;

protected:
    Document& associated_document_;
};

class AddCommand : public Command {
public:
    AddCommand(Document& doc);

    void Undo() override;
};

class DeleteCommand : public Command {
public:
    DeleteCommand(Document& doc, std::shared_ptr<Figure> fig, size_t index);

    void Undo() override;

private:
    size_t index_;
    std::shared_ptr<Figure> contained_figure_;
};

class Document {
    friend Command;
    friend AddCommand;
    friend DeleteCommand;
public:

    Document(const std::string& str);

    void SaveToFile() const;
    void LoadFromFile();

    void Render(const sdl::renderer& r);

    void AddFigure(std::shared_ptr<Figure>);
    void DeleteFigure(size_t index);
    void DeleteFigure(Point p);

    void SetColor(int r, int g, int b);

    void Undo();


private:
    std::string document_name_;
    std::vector<std::shared_ptr<Figure>> figures_;
    std::vector<std::shared_ptr<Command>> commands_;
    int rcolor = 255, gcolor = 0, bcolor = 0;
};