#include "Redactor.h"


void Redactor::CreateDocument(const std::string& document_name) {
    document = std::make_shared<Document>(document_name);
}

void Redactor::LoadDocument(const std::string &filename) {
    document = std::make_shared<Document>(filename);
    document->LoadFromFile();
}

void Redactor::Render(const sdl::renderer& r) {
    if (HasDocument()) {
        document->Render(r);
    }
}

void Redactor::SaveDocument() {
    if (HasDocument()) {
        document->SaveToFile();
    }
}

bool Redactor::HasDocument() const {
    return document != nullptr;
}

void Redactor::InsertFigure(std::shared_ptr<Figure> figure) {
    if (HasDocument()) {
        document->AddFigure(figure);
    }
}

void Redactor::RemoveFigureOnPoint(Point p) {
    if (HasDocument()) {
        document->DeleteFigure(p);
    }
}

void Redactor::RemoveFigureOnIndex(size_t index) {
    if (HasDocument()) {
        document->DeleteFigure(index);
    }
}

void Redactor::Undo() {
    if (HasDocument()) {
        document->Undo();
    }
}

void Redactor::SetColor(int r, int g, int b) {
    if (HasDocument()) {
        document->SetColor(r,g,b);
    }
}


