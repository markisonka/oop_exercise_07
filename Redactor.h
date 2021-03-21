#pragma once

#include <string>
#include <memory>

#include "FiguresAndFactories/Point.h"
#include "Document.h"

class Redactor {
public:

    void CreateDocument(const std::string& document_name);
    void LoadDocument(const std::string& filename);
    void SaveDocument();

    bool HasDocument() const;

    void Render(const sdl::renderer& r);
    void InsertFigure(std::shared_ptr<Figure> figure);
    void RemoveFigureOnPoint(Point p);
    void RemoveFigureOnIndex(size_t index);

    void SetColor(int r, int g, int b);

    void Undo();

private:
    std::shared_ptr<Document> document = nullptr;

};