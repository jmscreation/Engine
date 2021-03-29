#ifndef __TEST_APP_H__
#define __TEST_APP_H__

class TestItem;

class TestApplication : public Engine::App {
public:
    TestApplication();
    virtual ~TestApplication();

    static TestApplication& current() { return *static_cast<TestApplication*>(&Engine::App::current()); }

    sf::Image tempImg; // create an image for testing
    sf::Texture texture; // create a texture for the test item
    Engine::Animation* animation; // prepare an animation for the test item
    TestItem* item; // a place to store the test item
};

class TestItem : public Engine::GameObject , public Engine::Input::Mouse {
public:
    TestItem(float x=0, float y=0);
    virtual ~TestItem();

    inline virtual int type() { return 0; } // used for identifying between derived class GameObjects

    void mouseMove(sf::Event::MouseMoveEvent &evt);
};



#endif // __TEST_APP_H__
