#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "FiguresAndFactories/Factory.h"
#include "FiguresAndFactories/Circle/CircleFactory.h"
#include "FiguresAndFactories/Polygon/Trapeze/TrapezeFactory.h"
#include "FiguresAndFactories/Polygon/Rhombus/RhombusFactory.h"
#include "FiguresAndFactories/Polygon/RandomPolygon/RandomPolygonFactory.h"
#include "FiguresAndFactories/Polygon/Line/LineFactory.h"
#include "Redactor.h"
#include "sdl.h"
#include "imgui.h"

#define FILENAME_LENGTH 128

int main() {
    Redactor redactor;
    sdl::renderer renderer("Editor");
    bool quit = false;
    bool building_polygon = false;
    std::unique_ptr<Factory> active_builder = nullptr;
    char file_name[FILENAME_LENGTH] = "";
    std::string color_result;
    int32_t remove_id = 0;
    int red = 255, green = 0, blue = 0;
    bool remove_figure = false;

    while (!quit) {
        renderer.set_color(0, 0, 0);
        renderer.clear();

        sdl::event event;

        while (sdl::event::poll(event)) {
            sdl::quit_event quit_event;
            sdl::mouse_button_event mouse_button_event;
            if (event.extract(quit_event)) {
                quit = true;
                break;
            } else if (event.extract(mouse_button_event)) {
                if (active_builder && mouse_button_event.button() == sdl::mouse_button_event::left &&
                    mouse_button_event.type() == sdl::mouse_button_event::down) {
                    active_builder->AddPoint(Point{(double) mouse_button_event.x(), (double) mouse_button_event.y()});
                    if (dynamic_cast<RandomPolygonFactory*>(active_builder.get()) == nullptr
                        &&  dynamic_cast<LineFactory*>(active_builder.get()) == nullptr) {
                        std::shared_ptr<Figure> new_figure = nullptr;
                        try {
                            new_figure = active_builder->TryCreateObject();
                        } catch (...) {
                            active_builder = nullptr;
                        }
                        if (new_figure) {
                            redactor.InsertFigure(new_figure);
                            active_builder = nullptr;
                        }
                    }
                }
                if (mouse_button_event.button() == sdl::mouse_button_event::right && mouse_button_event.type() == sdl::mouse_button_event::down) {
                    if (active_builder && (dynamic_cast<RandomPolygonFactory*>(active_builder.get()) != nullptr
                        || dynamic_cast<LineFactory*>(active_builder.get()) != nullptr)) {
                        std::shared_ptr<Figure> new_figure = active_builder->TryCreateObject();
                        if (new_figure) {
                            redactor.InsertFigure(new_figure);
                            active_builder = nullptr;
                        }
                    } else if (!active_builder) {
                        redactor.RemoveFigureOnPoint(Point{(double)mouse_button_event.x(), (double) mouse_button_event.y()});
                    }

                }
            }
        }

        redactor.Render(renderer);

        ImGui::Begin("Menu");
        ImGui::InputText("File name", file_name, FILENAME_LENGTH - 1);
        if (ImGui::Button("Create file")) {
            std::string file_name_str(file_name);
            if (!file_name_str.empty()) {
                redactor.CreateDocument(file_name_str);
            }
        }
        if (ImGui::Button("Save file")) {
            if (redactor.HasDocument()) {
                redactor.SaveDocument();
            }
        }

        if (ImGui::Button("Load file")) {
            std::string filename(file_name);
            if (!filename.empty()) {
                redactor.LoadDocument(filename);
            }
        }
        if (ImGui::Button("Trapeze")) {
            active_builder = std::make_unique<TrapezeFactory>();
        }
        ImGui::SameLine();
        if (ImGui::Button("Circle")) {
            active_builder = std::make_unique<CircleFactory>();
        }
        if (ImGui::Button("Random Polygon")) {
            active_builder = std::make_unique<RandomPolygonFactory>();
        }
        ImGui::SameLine();
        if (ImGui::Button("Rhombus")) {
            active_builder = std::make_unique<RhombusFactory>();
        }
        if (ImGui::Button("Line")) {
            active_builder = std::make_unique<LineFactory>();
        }

        ImGui::InputInt("Remove id", &remove_id);
        if (ImGui::Button("Remove")) {
            redactor.RemoveFigureOnIndex(remove_id);
        }
        ImGui::LabelText("","Enter color %s", color_result.c_str());
        ImGui::InputInt("Red", &red);
        ImGui::InputInt("Green", &green);
        ImGui::InputInt("Blue", &blue);
        if (ImGui::Button("Set color")) {
            if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
                color_result = "Error, invalid color";
                red = 255;
                green = 0;
                blue = 0;
            } else {
                redactor.SetColor(red,green,blue);
                color_result = "";
            }
        }
        if (ImGui::Button("Undo")) {
            redactor.Undo();
        }
        ImGui::End();

        renderer.present();
    }
}
