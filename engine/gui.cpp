#include "engine.h"

namespace Engine {

    namespace Gui {
        //gui theme class
        Theme::Theme(const ComponentTheme& a, const CheckboxTheme& b, const TextboxTheme& c, const ButtonTheme& d):
            themeComponent(a), themeCheckbox(b), themeTextbox(c), themeButton(d) {}

        Theme::Theme(const Theme& a):
            themeComponent(a.themeComponent), themeCheckbox(a.themeCheckbox), themeTextbox(a.themeTextbox), themeButton(a.themeButton) {}

        Theme::~Theme() { }

        //main component class implementation
        Component::Component(Theme& theme, Callback cb): boundingBox({0,0,0,0}), x(0), y(0), disabled(false), currentTheme(theme), click(false), touch(TOUCH_NONE), callbackFunction(cb) {
            view().reset(sf::FloatRect(0, 0, App::current().window().getSize().x, App::current().window().getSize().y));
        }

        Component::~Component() {

        }

        bool Component::insideBox(float x, float y){
            return (x > boundingBox.left && x < boundingBox.left + boundingBox.width &&
                y > boundingBox.top && y < boundingBox.top + boundingBox.height);
        }

        bool Component::insideBox(const sf::Vector2f& pos){
            return insideBox(pos.x, pos.y);
        }

        void Component::drawScreenFront(sf::RenderWindow &win, sf::Time &delta) {
            draw(win);
        }

        void Component::setPosition(const sf::Vector2f& pos){
            x = pos.x; y = pos.y;
            updatePosition();
        }

        void Component::setPosition(float _x, float _y){
            x = _x; y = _y;
            updatePosition();
        }

        sf::Vector2f Component::getPosition(){
            return sf::Vector2f(x, y);
        }

        void Component::Disable(bool dsb){
            disabled = dsb;
        }

        void Component::mouseButtonPress(sf::Event::MouseButtonEvent &evt){
            sf::Vector2f pos(view().viewMouseX(), view().viewMouseY()),
                rpos(pos.x - x, pos.y - y);
            mousePress(pos, evt.button);
            if(disabled) return;
            click = insideBox(pos);
            if(click) onDown(rpos, evt.button);
        }

        void Component::mouseButtonRelease(sf::Event::MouseButtonEvent &evt){
            sf::Vector2f pos(view().viewMouseX(), view().viewMouseY()),
                rpos(pos.x - x, pos.y - y);
            mouseRelease(pos, evt.button);
            if(click){
                if(insideBox(pos)){
                    do{
                        if(callbackFunction != NULL) if(!callbackFunction((Engine::Gui::Component*)this, rpos, evt.button)) break;
                        onUp(rpos, evt.button);
                    } while(0);
                }
            }
            click = false;
        }

        void Component::touchBegin(sf::Event::TouchEvent &evt){
            sf::Vector2f pos(evt.x, evt.y),
                rpos(pos.x - x, pos.y - y);
            mousePress(pos, sf::Mouse::Left);
            if(insideBox(pos)) touch = evt.finger;
            if(touch) onDown(rpos, sf::Mouse::Left);
        }

        void Component::touchEnd(sf::Event::TouchEvent &evt){
            sf::Vector2f pos(evt.x, evt.y),
                rpos(pos.x - x, pos.y - y);
            mouseRelease(pos, sf::Mouse::Left);
            if(touch == evt.finger){
                if(insideBox(pos)){
                    do{
                        if(callbackFunction != NULL) if(!callbackFunction((Engine::Gui::Component*)this, rpos, sf::Mouse::Left)) break;
                        onUp(rpos, sf::Mouse::Left);
                    } while(0);
                }
                touch = TOUCH_NONE;
            }
        }

        //Extended components


        //Checkbox implementation

        Checkbox::Checkbox(Theme &theme, Callback cb):
            Component(theme, cb), checked(false), clicking(false) {
            sf::Vector2f size(theme.themeCheckbox.size);

            boundingBox.width = size.x;
            boundingBox.height = size.y;

            outside.setSize(size);
            outside.setFillColor(theme.themeComponent.color_back);
            outside.setOutlineThickness(theme.themeComponent.border);
            outside.setOutlineColor(theme.themeComponent.color_border);
            outside.setOrigin(size.x/2, size.y/2);

            overlay = outside;
            overlay.setFillColor(sf::Color(128, 128, 128, 128));

            check.setSize(sf::Vector2f(size.x/1.7, size.y/1.7));
            check.setFillColor(theme.themeCheckbox.color_check);
            check.setOrigin(check.getSize().x/2, check.getSize().y/2);
        }

        Checkbox::~Checkbox() {}

        void Checkbox::draw(sf::RenderWindow &win) {
            win.draw(outside);
            if(checked || clicking) win.draw(check);
            if(disabled) win.draw(overlay);
        }

        void Checkbox::updatePosition(){
            boundingBox.left = x - boundingBox.width/2;
            boundingBox.top = y - boundingBox.height/2;
            outside.setPosition(x, y);
            overlay.setPosition(x, y);
            check.setPosition(x, y);
        }


        void Checkbox::onDown(const sf::Vector2f& pos, sf::Mouse::Button btn){
            if(btn == sf::Mouse::Left){
                clicking = true;
                check.setFillColor(currentTheme.themeComponent.color_press);
            }
        }

        void Checkbox::mouseRelease(const sf::Vector2f& pos, sf::Mouse::Button btn) {
            check.setFillColor(currentTheme.themeCheckbox.color_check);
            clicking = false;
        }

        void Checkbox::onUp(const sf::Vector2f& pos, sf::Mouse::Button btn){
            if(btn == sf::Mouse::Left){
                Check(!Check());
            }
        }

        void Checkbox::Check(bool chk) {
            checked = chk;
        }

        //Textbox implementation

        Textbox::Textbox(Theme &theme, Callback cb):
            Component(theme, cb), textFont(theme.themeTextbox.font_text), selected(false), textString("") {
            sf::Vector2f size(theme.themeTextbox.size);
            boundingBox.width = size.x;
            boundingBox.height = size.y;

            outside.setSize(size);
            outside.setFillColor(theme.themeComponent.color_back);
            outside.setOutlineThickness(theme.themeComponent.border);
            outside.setOutlineColor(theme.themeComponent.color_border);
            outside.setOrigin(size.x/2, size.y/2);

            overlay = outside;
            overlay.setFillColor(theme.themeComponent.color_disabled);
            if(theme.themeTextbox.font_text != NULL)
                textObj.setFont(*theme.themeTextbox.font_text);
            if(!theme.themeTextbox.font_size)
                textObj.setCharacterSize(size.y/1.5);
            else
                textObj.setCharacterSize(theme.themeTextbox.font_size);
            textObj.setOrigin(size.x/2-2, size.y/2);
            textObj.setFillColor(theme.themeTextbox.color_text);

            textCursor.setSize(sf::Vector2f(0.2 + size.y/25, size.y/1.5));
            textCursor.setFillColor(theme.themeTextbox.color_cursor);
            textCursor.setOrigin(size.x/2-2 - size.y/25, size.y/3);
            updateString();
        }

        Textbox::~Textbox() {}

        void Textbox::Disable(bool dsb) {
            disabled = dsb;
            selected = disabled ? false : selected;
        }

        void Textbox::draw(sf::RenderWindow &win) {
            win.draw(outside);
            win.draw(textObj);
            if(selected) win.draw(textCursor);
            if(disabled) win.draw(overlay);
        }

        void Textbox::updatePosition() {
            boundingBox.left = x - boundingBox.width/2;
            boundingBox.top = y - boundingBox.height/2;
            textObj.setPosition(x, y);
            outside.setPosition(x, y);
            overlay.setPosition(x, y);
            updateCursor();
        }

        void Textbox::updateCursor() {
            textCursor.setPosition(x + textObj.getGlobalBounds().width, y);
        }

        void Textbox::onDown(const sf::Vector2f& pos, sf::Mouse::Button btn){
            outside.setOutlineColor(currentTheme.themeComponent.color_press);
        }

        void Textbox::onUp(const sf::Vector2f& pos, sf::Mouse::Button btn) {
            if(btn == sf::Mouse::Left){
                Select();
            }
        }

        void Textbox::mouseRelease(const sf::Vector2f& pos, sf::Mouse::Button btn){
            if(btn == sf::Mouse::Left){
                selected = false;
            }
            outside.setOutlineColor(currentTheme.themeComponent.color_border);
        }

        void Textbox::setString(const std::string& str){
            textString = str;
            updateString();
        }

        std::string Textbox::getString(){
            return textString;
        }

        void Textbox::keyPress(sf::Event::KeyEvent &evt){
            if(!selected) return;
            switch(evt.code){
                case sf::Keyboard::Enter:
                    onEnter(textString);
                break;
                case sf::Keyboard::Tab:
                    onTab(textString);
                break;
            }
            updateString();
        }

        void Textbox::windowTextEntered(sf::Event::TextEvent &evt){
            if(!selected) return;
            if((evt.unicode < 127 && evt.unicode > 31)){
                textString += evt.unicode;
            }
            if(evt.unicode == 8 && textString.length()){
                textString.pop_back();
            }
            updateString();
        }

        void Textbox::Select() {
            if(!disabled){
                selected = true;
                updateString();
            }
        }

        void Textbox::Deselect() {
            selected = false;
        }

        void Textbox::updateString(){
            textObj.setString(textString);
            while(textObj.getGlobalBounds().width > boundingBox.width - 8){
                textString = std::string(textObj.getString(), 0, textObj.getString().getSize() - 1);
                textObj.setString(textString);
            }
            textString = textObj.getString();
            updateCursor();
        }

        //Button implementation

        Button::Button(Theme& theme, Callback cb, const std::string& btnString): Component(theme, cb),
            textFont(theme.themeButton.font_text) {
            sf::Vector2f size(theme.themeButton.size);
            boundingBox.width = size.x;
            boundingBox.height = size.y;

            outside.setSize(size);
            outside.setFillColor(theme.themeComponent.color_back);
            outside.setOutlineThickness(theme.themeComponent.border);
            outside.setOutlineColor(theme.themeComponent.color_border);
            outside.setOrigin(size.x/2, size.y/2);

            overlay = outside;
            overlay.setFillColor(theme.themeComponent.color_disabled);

            if(theme.themeButton.font_text != NULL)
                textObj.setFont(*theme.themeButton.font_text);
            textObj.setFillColor(theme.themeButton.color_text);
            if(!theme.themeButton.font_size)
                textObj.setCharacterSize(250);
            else
                textObj.setCharacterSize(theme.themeButton.font_size);
            setString(btnString);
        }

        Button::~Button() {}

        void Button::draw(sf::RenderWindow &win){
            win.draw(outside);
            win.draw(textObj);
            if(disabled) win.draw(overlay);
        }

        void Button::updatePosition(){
            boundingBox.left = x - boundingBox.width/2;
            boundingBox.top = y - boundingBox.height/2;
            textObj.setPosition(x, y);
            outside.setPosition(x, y);
            overlay.setPosition(x, y);
        }

        void Button::onDown(const sf::Vector2f& pos, sf::Mouse::Button btn) {
            if(disabled) return;
            if(btn == sf::Mouse::Left){
                outside.setFillColor(currentTheme.themeComponent.color_press);
            }
        }

        void Button::mouseRelease(const sf::Vector2f& pos, sf::Mouse::Button btn) {
            outside.setFillColor(currentTheme.themeComponent.color_back);
        }

        std::string Button::getString() {
            return textObj.getString();
        }

        void Button::setString(const std::string& str){
            textObj.setString(str);
            while(textObj.getLocalBounds().width > boundingBox.width - boundingBox.height/8){
                textObj.setCharacterSize(textObj.getCharacterSize() - 1);
            }
            textObj.setOrigin(textObj.getLocalBounds().left + textObj.getLocalBounds().width / 2, textObj.getLocalBounds().top + textObj.getLocalBounds().height / 2);
        }

    }

}
