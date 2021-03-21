#include "Document.h"

Command::Command(Document& doc)
: associated_document_(doc) {}

Document::Document(const std::string &str)
: document_name_(str){}

AddCommand::AddCommand(Document& doc)
: Command(doc) {}

void AddCommand::Undo() {
    associated_document_.figures_.pop_back();
}

DeleteCommand::DeleteCommand(Document& doc, std::shared_ptr<Figure> fig, size_t index)
: Command(doc), contained_figure_(move(fig)), index_(index) {}

void DeleteCommand::Undo() {
    auto it = associated_document_.figures_.begin();
    associated_document_.figures_.insert(it + index_, contained_figure_);
}


void Document::AddFigure(std::shared_ptr<Figure> fig) {
    figures_.push_back(move(fig));
    figures_.back()->SetColor(rcolor, gcolor, bcolor);
    commands_.push_back(std::make_shared<AddCommand>(*this));

}

void Document::DeleteFigure(Point p) {
    for (size_t i = 0; i < figures_.size(); ++i) {
        size_t cur_index = figures_.size() - i - 1;
        if (figures_[cur_index]->PointBelongsTo(p)) {
            DeleteFigure(cur_index);
            break;
        }
    }
}

void Document::DeleteFigure(size_t index) {
    if (index >= figures_.size()) {
        return;
    }
    auto ptr = std::make_shared<DeleteCommand>(*this, figures_[index], index);
    figures_.erase(figures_.begin() + index);
    commands_.push_back(ptr);

}

void Document::Render(const sdl::renderer& r) {
    for (auto i : figures_) {
        i->Render(r);
    }
}

void Document::Undo() {
    if (commands_.empty()) {
        return;
    }
    std::shared_ptr<Command> ptr = commands_.back();
    commands_.pop_back();
    ptr->Undo();
}

void Document::SetColor(int r, int g, int b) {
    rcolor = r;
    gcolor = g;
    bcolor = b;
}

void Document::SaveToFile() const {
    std::ofstream os(document_name_, std::ios::out | std::ios::trunc);
    if (!os.is_open()) {
        return;
    }
    os << figures_.size() << "\n";
    for (auto ptr : figures_) {
        ptr->Save(os);
    }
    os.close();
}

void Document::LoadFromFile() {
    std::ifstream is(document_name_, std::ios::in);
    if (!is) {
        return;
    }
    figures_.clear();
    commands_.clear();
    size_t figures_count;
    is >> figures_count;
    for (size_t i = 0; i < figures_count; ++i) {
        std::string type;
        is >> type;
        std::shared_ptr<Figure> ptr;
        if (type == "circle") {
            ptr = std::make_shared<Circle>();
        } else if (type == "random_polygon") {
            ptr = std::make_shared<RandomPolygon>();
        } else if (type == "trapeze") {
            ptr = std::make_shared<Trapeze>();
        } else  if (type == "rhombus") {
            ptr = std::make_shared<Rhombus>();
        }
        ptr->Load(is);
        figures_.push_back(ptr);
    }
    is.close();

}