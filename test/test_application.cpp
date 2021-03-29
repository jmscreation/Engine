#include "includes.h"

/**
    Test Application Class
**/

//start of application
TestApplication::TestApplication(): Engine::App(600, 400, "Test Application", sf::Style::Default &~ sf::Style::Resize) {
    std::cout << "Application begin" << std::endl;

    tempImg.create(128, 128); // create a blank image

    for(size_t x=0;x < tempImg.getSize().x; ++x){ // randomize the pixels in the image
        for(size_t y=0;y < tempImg.getSize().y; ++y){
            tempImg.setPixel(x, y, sf::Color( rand() % 256, rand() % 256, rand() % 256, (x * 2) % 256));
        }
    }

    texture.loadFromImage(tempImg); // load the image into a texture

    animation = new Engine::Animation(&texture, {4}, 0, 0, 64, 64, 2); // create an animation for test item from the texture
    // animation has 4 frames, starting at 0 0, size 64x64, 2 frames-per-row

    item = new TestItem(16, 16); // create the test item
}

//End of application
TestApplication::~TestApplication(){
    std::cout << "Application closed" << std::endl;
    delete item; // destroy item
    delete animation; // destroy animation - always destroy animations after the GameObject instances
}


/**
    Test Item Class
**/

// Item created
TestItem::TestItem(float x, float y): GameObject(nullptr, x, y) {
    sprite.updateAnimation(TestApplication::current().animation); // update my current animation
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f); // center origin
}

// Item destroyed
TestItem::~TestItem() {}

/// Mouse Move Event
void TestItem::mouseMove(sf::Event::MouseMoveEvent &evt){
    const Engine::View& view = Engine::App::current().view(); // get the default view

    setPosition(view.viewMouseX(), view.viewMouseY()); // update position to mouse position

    sprite.next(); // update animation frame
}
