#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;


struct Point
{
    int x, y;
};


class DoodlePlayer
{
public:
    float x, y, h;
    float dx, dy;
    Sprite doodleSprite;


    DoodlePlayer(Texture &texture, float startX, float startY, float startH) : x(startX), y(startY), h(startH), dx(0), dy(0)
    {
        doodleSprite.setTexture(texture);
        doodleSprite.setPosition(x, y);
    }


    void move(bool moveRight, bool moveLeft)
    {
        if (moveRight)
            x += 3;
        if (moveLeft)
            x -= 3;


        if (x > 400)
            x = -doodleSprite.getGlobalBounds().width;
        else if (x < -doodleSprite.getGlobalBounds().width)
            x = 400;
    }


    void update()
    {
        dy += 0.2;
        y += dy;
        doodleSprite.setPosition(x, y);
    }
};


bool checkCollision(DoodlePlayer &player, Point &platform)
{
    return (player.x + 50 > platform.x) && (player.x + 20 < platform.x + 68) &&
           (player.y + 70 > platform.y) && (player.y + 70 < platform.y + 14) && (player.dy > 0);
}


int main()
{
    srand(time(0));


    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    app.setFramerateLimit(60);


    Texture backgroundTexture, platformTexture, doodleTexture, powerupTexture, titleTexture;
    backgroundTexture.loadFromFile("background.png");
    platformTexture.loadFromFile("platform.png");
    doodleTexture.loadFromFile("Duck.png");
    powerupTexture.loadFromFile("powerup.png");
    titleTexture.loadFromFile("title.png");


    Font gameFont;
    gameFont.loadFromFile("muffins.ttf");


    Sprite backgroundSprite(backgroundTexture), platformSprite(platformTexture), doodleSprite(doodleTexture), powerupSprite(powerupTexture), titleSprite(titleTexture);


    RectangleShape blackScreen(Vector2f(app.getSize().x, app.getSize().y));
    blackScreen.setFillColor(Color::Black);


    std::vector<Point> platforms(20);
    for (int i = 0; i < 10; i++)
    {
        platforms[i].x = rand() % 400;
        platforms[i].y = rand() % 533;
    }


    int score = 0;
    int countdown = 50;


    Text scoreText, deathText1, deathText2, deathText3, countdownText, startText;
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(25);
    scoreText.setPosition(10, 10);
    scoreText.setFillColor(Color::Red);


    deathText1.setFont(gameFont);
    deathText1.setCharacterSize(30);
    deathText1.setPosition(50, 100);
    deathText1.setFillColor(Color::Red);
    deathText1.setString("You Died!");


    deathText2.setFont(gameFont);
    deathText2.setCharacterSize(30);
    deathText2.setFillColor(Color::Red);
    deathText2.setPosition(50, 200);
    deathText2.setString("Press R to Restart");


    deathText3.setFont(gameFont);
    deathText3.setCharacterSize(30);
    deathText3.setFillColor(Color::Red);
    deathText3.setPosition(50, 300);


    countdownText.setFont(gameFont);
    countdownText.setCharacterSize(25);
    countdownText.setFillColor(Color::Red);
    countdownText.setPosition(225, 10);


    startText.setFont(gameFont);
    startText.setCharacterSize(40);
    startText.setFillColor(Color::White);
    startText.setPosition(150, 300);
    startText.setString("Start");


    bool deathScreen = false;
    bool powerupActive = false;


    bool titleScreen = true;
    bool startGame = false;


    DoodlePlayer player(doodleTexture, 100, 100, 200);
   
    int myarray[10] = { 12,4,3,1,15,45,33,21,10,2};   
         
    for(int k=1; k<10; k++)   
    {  
        int temp = myarray[k];  
        int j= k-1;  
        while(j>=0 && temp <= myarray[j])  
        {  
            myarray[j+1] = myarray[j];   
            j = j-1;  
        }  
        myarray[j+1] = temp;  
    } 

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
            else if (e.type == Event::KeyPressed && e.key.code == Keyboard::R)
            {
                app.clear();
                app.draw(blackScreen);
                app.display();


                player.y = player.h;
                player.dy = 0;
                score = 0;
                countdown = 50;
                deathScreen = false;
                powerupActive = false;
                titleScreen = true;
                startGame = false;
            }
            else if (e.type == Event::MouseButtonPressed && titleScreen)
            {


                if (Mouse::getPosition(app).x >= 120 && Mouse::getPosition(app).x <= 280 &&
                    Mouse::getPosition(app).y >= 300 && Mouse::getPosition(app).y <= 350)
                {
                    titleScreen = false;
                    startGame = true;
                }
            }
        }


        if (titleScreen)
        {
            app.clear();
            app.draw(titleSprite);
            app.draw(startText);
            app.display();
        }
        else if (!deathScreen)
        {
            if (startGame)
            {
                player.move(Keyboard::isKeyPressed(Keyboard::Right), Keyboard::isKeyPressed(Keyboard::Left));
                player.update();


                float dy = player.dy;
                float y = player.y;


                if (y > 533)
                {
                    deathScreen = true;
                }


                if (y < player.h)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        player.y = player.h;
                        platforms[i].y = platforms[i].y - dy;
                        if (platforms[i].y > 533)
                        {
                            platforms[i].y = 0;
                            platforms[i].x = rand() % 400;
                            score++;
                            countdown--;


                            if (countdown == 0)
                            {
                                deathScreen = true;
                            }


                            if (countdown % 10 == 0 && !powerupActive)
                            {
                                int powerupPlatformIndex = rand() % 10;
                                powerupSprite.setPosition(platforms[powerupPlatformIndex].x, platforms[powerupPlatformIndex].y - 30);
                                powerupActive = true;                             }
                        }
                    }
                }


                for (int i = 0; i < 10; i++)
                    if (checkCollision(player, platforms[i]))
                        player.dy = -10;


                if (powerupActive && player.doodleSprite.getGlobalBounds().intersects(powerupSprite.getGlobalBounds()))
                {
                    countdown += 10;
                    powerupActive = false;
                }


                if (powerupActive)
                    powerupSprite.setPosition(platforms[0].x, platforms[0].y - 30);


                powerupSprite.setPosition(powerupSprite.getPosition().x, powerupSprite.getPosition().y + dy);


                app.clear();
                app.draw(backgroundSprite);
                app.draw(powerupSprite);
                app.draw(player.doodleSprite);
                for (int i = 0; i < 10; i++)
                {
                    platformSprite.setPosition(platforms[i].x, platforms[i].y);
                    app.draw(platformSprite);
                }


                scoreText.setString("Score: " + std::to_string(score));
                app.draw(scoreText);


                countdownText.setString("Countdown: " + std::to_string(countdown));
                app.draw(countdownText);
            }
        }
        else
        {
            app.clear();
            app.draw(deathText1);
            app.draw(deathText2);
            deathText3.setString("Score: " + std::to_string(score));
            app.draw(deathText3);
        }


        app.display();
    }


    return 0;
}
