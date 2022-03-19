#include "header.hpp"

#if !defined(__unix__) && !defined(__unix)
#include <windows.h>
#endif

sf::RenderWindow window;
sf::Image icon;

#if defined(__unix__) || defined(__unix)
int main(int argc, char ** argv) {
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#endif

    	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bongo Cat", sf::Style::Titlebar | sf::Style::Close);
    	/*msg("窗口已创建。");*/
    	printf("[Bongo Cat]窗口已创建。\n");
	icon.loadFromFile("files/icon.jpg"); 
    	printf("[Bongo Cat]图标已导入。\n");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()); 
    	printf("[Bongo Cat]图标已设置。\n");
    	window.setFramerateLimit(MAX_FRAMERATE);

    	// loading configs
    	while (!data::init()) {
        	continue;
    	}

    // initialize input
    if (!input::init()) {
        return EXIT_FAILURE;
    }

    bool is_reload = false;
    bool is_show_input_debug = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            	case sf::Event::Closed:
                	window.close();
    			printf("[Bongo Cat]窗口已关闭。\n");
                	break;

            	case sf::Event::KeyPressed:
                	// get reload config prompt
                	if (event.key.code == sf::Keyboard::R && event.key.control) {
                    		if (!is_reload) {
                        		while (!data::init()) {
                            			continue;
                        		}
                    		}
                    		is_reload = true;
                    		break;
                	}

                	// toggle joystick debug panel
                	if (event.key.code == sf::Keyboard::D && event.key.control) {
                    		is_show_input_debug = !is_show_input_debug;
                    		break;
                	}

            	default:
                	is_reload = false;
            }
        }

        int mode = data::cfg["mode"].asInt();

        Json::Value rgb = data::cfg["decoration"]["rgb"];
        int red_value = rgb[0].asInt();
        int green_value = rgb[1].asInt();
        int blue_value = rgb[2].asInt();
        int alpha_value = rgb.size() == 3 ? 255 : rgb[3].asInt();

        window.clear(sf::Color(red_value, green_value, blue_value, alpha_value));
        switch (mode) {
        case 1:
            osu::draw();
            break;
        case 2:
            taiko::draw();
            break;
        case 3:
            ctb::draw();
            break;
        case 4:
            mania::draw();
            break;
        case 5:
            custom::draw();
        }

        if (is_show_input_debug) {
            input::drawDebugPanel();
        }

        window.display();
    }

    input::cleanup();
    printf("\n");
    return 0;
}

