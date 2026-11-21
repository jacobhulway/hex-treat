//OOH look how organized it is
//i "optimized" it into like 50 lines :)
//im so talented
#pragma region include statements whatever that means
#include <glad/gl.h>
#include<GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<vector>
#include<ranges>
#include<algorithm>
#include<windows.h>
#include<mmsystem.h>
#pragma endregion
//hi

#pragma region super basic game variables
float targetFPS = 60;
double targetFrameTime = 1.0 / targetFPS;
double lastFrameTime = glfwGetTime();

bool drawHitboxes = false;

struct point {
    float x;
    float y;
    point(float x, float y) : x(x), y(y) {}
    point() {}
};

int randBool() {
	return rand() % 2;
}

float playerX;
float playerY;

float playerRadius = 8;

float playerHitboxOffsetX = 0;
float playerHitboxOffsetY = 8;

double mouseX = 0, mouseY = 0;
double mouseRelX = 0, mouseRelY = 0;
double prevMouseX = 0, prevMouseY = 0;

float cursorAngle = 0;
//shh
float secretPlayerX, secretPlayerY;

bool mouseDown = false;
bool prevMouseDown = false;
bool newClick = false;

#pragma endregion

#pragma region Texture Bites
unsigned int texture;
int textureWidth, textureHeight;
using namespace std;
//keep everything here :)
vector<vector<int>> textureStorage = {
    {0, 0, 200, 200},

    {0, 200, 200, 200},

    {0, 400, 200, 198},

    {0, 598, 280, 141},


    {500, 0, 23, 45},

    {0, 3916, 320, 180},

    {320, 3916, 320, 180},

    {640, 3916, 320, 180}
    


}; //write these bite manualy

int getTextureWidth(int ID) {
    return textureStorage[ID][2];
}
int getTextureHeight(int ID) {
    return textureStorage[ID][3];
}

vector<vector<float>> calculatedTextureStorage = {}; //leave this one blank so the function can do the work :o

//sets up all the texture terats :)
vector<vector<float>> doTheTextureBites(vector<vector<int>> original, int width, int height) {
    vector<vector<float>> output = {};
    
    float w = width;
    float h = height;
    for (const vector<int> rect : original) {
        output.push_back({rect[0]/w, rect[1]/h, rect[2]/w, rect[3]/h});//i hope this works :)
    }
    return output;
}



//use all the formating treats for all of them so its clean ig

int __DIRT__ = 0;

int __MOSS__ = 1;

int __HORIZONTALROAD__ = 2;

int __VERTICALROAD__ = 3;

int __ELEVATOR__ = 4;

int __TITLE1FRAME1__ = 5;

int __TITLE1FRAME2__ = 6;

int __TITLE1FRAME3__ = 7;

//bro i give up on sorting its okay atp :\

//so empte :/

#pragma endregion
//is it inificient to put a 4096 by 4096 image on gee pee you? :/
#pragma region Font Bites
//font engine here V

//basic font stuff
int fontX = 200;
int fontY = 0;
float fontHeight = 12.0f / 4096.0f;
//holds a letter, its position and its width
struct character {
    char letter;
    float x;
    float widtht;
    float widthp;
    character(char letter, float x, float widthp, float widtht) : letter(letter), x(x), widthp(widthp), widtht(widtht) {}
    character();
};
//defines a special character width for character initialization
struct scw {
    char letter;
    int width;
    scw(char letter, int width) : letter(letter), width(width) {}
    scw() {}
};
//all of them :D
vector<char> letters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','.',',','(',')','/'};
//these are the special ones :/
vector<scw> oddLetters = { {'w', 5}, {'W', 5}, {'m', 5}, {'M', 5}, {'j', 2}, {'l', 1}, {'1', 2}, {'(', 2}, {')', 2}, {'.', 1}, {',', 1}};
//this is the list of in depth characters
vector<character> characters;
int averageWidth = 3;
//create the character list thing
int x = 0;
int seperation = 1;
//actualy creates the list of cgharacters that works for the font engine
void initFont() {
    for (char l : letters) {
        auto odd = std::ranges::find(oddLetters, l, &scw::letter);
        if (odd == oddLetters.end()) {
            characters.push_back(character(l, (float)fontX / (float)textureWidth + (float)x / (float)textureWidth, averageWidth, (float)averageWidth / (float)textureWidth));
            x += averageWidth + seperation;
        }
        else {
            characters.push_back(character(l, (float)fontX / (float)textureWidth + (float)x / (float)textureWidth, odd->width, (float)odd->width / (float)textureWidth));
            x += odd->width + seperation;
        }
        
    }
}
//gets the width of a character
int getCharWidth(char l) {
    if (l == ' ') {
        return 1;
    }
    auto ch = std::ranges::find(characters, l, &character::letter);
    if (ch == characters.end()) {
        return averageWidth;
    }
	return ch->widthp;
}
//draws one letter
int drawCharacter(float x, float y, char l) {
    if (l == ' ') {
        return 1;
    }
    auto ch = std::ranges::find(characters, l, &character::letter);
    if (ch == characters.end()) {
        return averageWidth;
    }


    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    glTexCoord2f(ch->x, 0);
    glVertex2f(x, y);

    glTexCoord2f(ch->x + ch->widtht, 0);
    glVertex2f(x + ch->widthp, y);

    glTexCoord2f(ch->x + ch->widtht, 0+  fontHeight);
    glVertex2f(x + ch->widthp, y + 12);

    glTexCoord2f(ch->x, 0+fontHeight);
    glVertex2f(x, y + 12);

    glEnd(); //so proud :D
    return ch->widthp;
}
//add m,ulti line suport in future please :/
//draws a texctBox
void drawText(float x, float y, string text, int max = 1000, float spacing = 1) {
    max += x;
    float startX = x;
    for (char l : text) {
        
        x+=drawCharacter(x, y, l);
        x += spacing;
        if (max != -1 and x > max) {
            x = startX;
            y += 12;
        }
    }
}
//does the same thing as the draw text function just dosent draw anything, just calculates the girthyness
int textWidth(string text) {
    int x = 0;
    for (char l : text) {
        x += getCharWidth(l) + 1;
    }
    return x;
}
//holds a text mesage to show up in mid game, stored inside game class i think
struct dialogue {
    string text;
    bool animate;
    int animationStage = 0;
    float r, b, g, a;
    
    int time;
    int length;
    dialogue(string text, int seconds, bool animate = true, float r = 1, float g = 1, float b = 1) : text(text), time(seconds * 60), animate(animate), r(r), g(g), b(b) {
        length = text.size();
        a = 1;
    }

    bool tick() {
        if (animate) {
            animationStage += 1;
            if (animationStage > length) {
                animate = false;
            }
        }
        time -= 1;
        if (time < 0) {
            a -= 0.01;
            if (a < 0) {
                return true;
            }
            else {
                return false;
            }

        }
        return false;

    }
    void draw() {
        int y = 140;
        string displayText = text;
        if (animate) {
            displayText.erase(animationStage);
        }
        glColor4f(r, g, b, a);
        drawText(20, y, displayText, 280);
        glColor4f(1, 1, 1, 1);

    }
};

#pragma endregion

#pragma region audio initialization and fuinctions
wstring toWstring(const std::string& str) {
    return wstring(str.begin(), str.end());
}
struct soundINIT {
	string alias;
	string path;
	soundINIT(string name, string path) : alias(name), path(path){}
};

vector<soundINIT> sounds = {soundINIT("boom","\"audio\\boom.mp3\""), soundINIT("bg1", "\"audio\\moo.mp3\"")};

vector<string> bgMusic = { "bg1", "boom"};


void playSound(string alias) {
    MCIERROR error = mciSendString((L"play " + toWstring(alias) + L" from 0").c_str(), NULL, 0, NULL);
	cout << "PLAYING SOUND: " << alias << endl;
    if (error) {
        wchar_t errorText[256];
        mciGetErrorString(error, errorText, 256);
        std::wcerr << errorText << std::endl;
    }
    else {
        std::cout << "PLAYING SOUND!" << std::endl;
    }
}

void loadSound(string path, string alias){
    MCIERROR error = mciSendString(toWstring("open " + path + " type mpegvideo alias " + alias).c_str(), NULL, 0, NULL);
    if (error) {
        wchar_t errorText[256];
        mciGetErrorString(error, errorText, 256);
        std::wcerr << errorText << std::endl;
    }
    else {
        std::cout << "Sucessfully Loaded Sound: " << alias << std::endl;
    }
}

void loadSounds() {
    for (soundINIT& s : sounds) {
        loadSound(s.path, s.alias);
    }
}

void bgMusic

#pragma endregion

#pragma region player modeel stuff:

//converts degrees to radians
double degreesToRadians(double degrees) {
    return degrees * (3.1416 / 180.0);
}
//playewr darwing stuff right here
struct specialPoint {
    float x;
    float y;
    float z;

    float rx;
    float ry;
    float rz;


};
//initializes a special 3d point used for animations
void initPoint(specialPoint& p, float x, float y, float z) {
    p.x = x;
    p.y = y;
    p.z = z;
    p.rx = x;
    p.ry = y;
    p.rz = z;



}
//rotates special 3d point around 0, 0
void rotateSpecialPointAlongZAxis(specialPoint& p, float angle) {
    p.rx = p.x * cos(angle) - p.y * sin(angle);
    p.ry = p.x * sin(angle) + p.y * cos(angle);
    p.rz = p.z;
}
//stores a single point keyframe for an animation
struct pkf {
    float x;
    float y;
    float z;
    specialPoint& p;
    pkf(float x, float y, float z, specialPoint& p) : x(x), y(y), z(z), p(p) {}

    
};
//stores one frame of an animation
struct animationKeyFrame {
    vector<pkf> akf;
    animationKeyFrame(vector<pkf> animationFrame) : akf(animationFrame) {}
    animationKeyFrame() {}


};
//applys a single points keyframe
void applyPointKeyFrame(pkf& frame, float strength) {
    frame.p.x += frame.x * strength;
    frame.p.y += frame.y * strength;
    frame.p.z += frame.z * strength;
}
//applys an animation frame to a player model
void applyEntireKeyFrame(animationKeyFrame& frame, float strength) {
    for (pkf& f : frame.akf) {
        applyPointKeyFrame(f, strength);
    }
}
//holds an entire animation
struct animation {
    vector<animationKeyFrame> frames;
    animationKeyFrame reset;
    float speed = 0.08;
    float frameNumber = 0;
    bool justReset = true;
    bool firstFrame = true;
    animation(vector<animationKeyFrame> frames, animationKeyFrame reset) : frames(frames), reset(reset) {}
    animation() {}
    int frameCount = frames.size();
    bool step() {
        frameNumber += speed;
        if (frameNumber > frameCount) {
            frameNumber = 0;
            justReset = true;
            return true;
        }
        else {
            justReset = false;
            return false;
        }

    }
    void apply() {
        
        if (justReset or firstFrame) {
            applyEntireKeyFrame(reset, 1);
        }
        int f = int(frameNumber);
        
        applyEntireKeyFrame(frames[f], speed);
        firstFrame=false;
    }
};
// the 3d model of the player
struct playerModel {
    specialPoint head, butt, leftElbow, leftHand, rightElbow, rightHand, leftKnee, leftFoot, rightKnee, rightFoot;
    
    float angle = 0;
    
    void reset() {
        initPoint(head, 0, 0, 10);
        initPoint(butt, 0, 0, 0);
        initPoint(rightKnee, 1, 0, -7);
        initPoint(rightFoot, 1.5, 0, -14);
        initPoint(leftKnee, -1, 0, -7);
        initPoint(leftFoot, -1.5, 0, -14);
        initPoint(rightElbow, 2, 0, 7);
        initPoint(rightHand, 3, 0, -3);
        initPoint(leftElbow, -2, 0, 7);
        initPoint(leftHand, -3, 0, -3);
    }
    playerModel() {
        reset();
    }

    animation motion = animation(
        {
            animationKeyFrame({
                pkf(0, -3, -2, leftKnee),
                pkf(0, -4, -2, leftFoot),
                pkf(0, 3, 0, rightKnee),
                pkf(0, 5, 0, rightFoot),
                pkf(0, 1, 1, leftElbow),
                pkf(0, 3, 2, leftHand),
                pkf(0, -1, 1, rightElbow),
                pkf(0, -3, 2, rightHand)

                }),

            animationKeyFrame({
                pkf(0, 3, 0, leftKnee),
                pkf(0, 5, 0, leftFoot),
                pkf(0, -3, 2, rightKnee),
                pkf(0, -6, 2, rightFoot),
                pkf(0, -1, -1, leftElbow),
                pkf(0, -3, -2, leftHand),
                pkf(0, 1, -1, rightElbow),
                pkf(0, 3, -2, rightHand)
                }),
            animationKeyFrame({
                pkf(0, 3, 0, leftKnee),
                pkf(0, 5, 0, leftFoot),
                pkf(0, -3, -2, rightKnee),
                pkf(0, -4, -2, rightFoot),
                pkf(0, -1, 1, leftElbow),
                pkf(0, -3, 2, leftHand),
                pkf(0, 1, 1, rightElbow),
                pkf(0, 3, 2, rightHand)
                }),
            animationKeyFrame({
                pkf(0, -3, 2, leftKnee),
                pkf(0, -6, 2, leftFoot),
                pkf(0, 3, 0, rightKnee),
                pkf(0, 5, 0, rightFoot),
                pkf(0, 1, -1, leftElbow),
                pkf(0, 3, -2, leftHand),
                pkf(0, -1, -1, rightElbow),
                pkf(0, -3, -2, rightHand)
                })
        },

        animationKeyFrame({
            pkf(0, 0, 2, leftKnee),
            pkf(0, 0, 2, leftFoot),
            })
    );
        

    
    void setAngle(float a) {
        angle = a;
        rotateSpecialPointAlongZAxis(head, a);
        rotateSpecialPointAlongZAxis(butt, a);
        rotateSpecialPointAlongZAxis(rightKnee, a);
        rotateSpecialPointAlongZAxis(rightFoot, a);
        rotateSpecialPointAlongZAxis(leftKnee, a);
        rotateSpecialPointAlongZAxis(leftFoot, a);
        rotateSpecialPointAlongZAxis(rightElbow, a);
        rotateSpecialPointAlongZAxis(rightHand, a);
        rotateSpecialPointAlongZAxis(leftElbow, a);
        rotateSpecialPointAlongZAxis(leftHand, a);

    }
    void changeAngle(float a) {
        a = angle + a;
        angle = a;
        rotateSpecialPointAlongZAxis(head, a);
        rotateSpecialPointAlongZAxis(butt, a);
        rotateSpecialPointAlongZAxis(rightKnee, a);
        rotateSpecialPointAlongZAxis(rightFoot, a);
        rotateSpecialPointAlongZAxis(leftKnee, a);
        rotateSpecialPointAlongZAxis(leftFoot, a);
        rotateSpecialPointAlongZAxis(rightElbow, a);
        rotateSpecialPointAlongZAxis(rightHand, a);
        rotateSpecialPointAlongZAxis(leftElbow, a);
        rotateSpecialPointAlongZAxis(leftHand, a);
    }
    

};
//the one :)
playerModel player = playerModel();

#pragma endregion

#pragma region 3d rendering stuff (also some screen initialization)


float cameraAngle = degreesToRadians(45);

float zRatio = sin(cameraAngle);

float yRatio = cos(cameraAngle);
//changes the required variables to change the 3d rendering angle
void updateCameraAngle(float angle) {
    cameraAngle = degreesToRadians(angle);

    zRatio = sin(cameraAngle);
    yRatio = cos(cameraAngle);
}
//projects a poijt in  3d space
point projectPoint(float x, float y, float z) {
    return point(x, y * yRatio - z * zRatio);
}

//put display size here when i add it --------------------------

int displayWidth = 2560;
int displayHeight = 1440;

int screenHeight = 180;
int screenWidth = 320;

GLFWwindow * window;

float ratio = (float)screenWidth / (float)displayWidth;

int centerX = screenWidth / 2;
int centerY = screenHeight / 2;

#pragma endregion

#pragma region esential drawing functions
void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);

    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);

    glEnd();
}
void drawSprite(unsigned int geepeeyouOne, int texturePath, float x, float y, float width, float height) {
    float tx, ty, tw, th;
    tx = calculatedTextureStorage[texturePath][0];

    ty = calculatedTextureStorage[texturePath][1];

    tw = calculatedTextureStorage[texturePath][2];

    th = calculatedTextureStorage[texturePath][3];

    
    glBindTexture(GL_TEXTURE_2D, geepeeyouOne);
    glBegin(GL_QUADS);

    glTexCoord2f(tx, ty);
    glVertex2f(x, y);

    glTexCoord2f(tx+tw, ty);
    glVertex2f(x+width, y);

    glTexCoord2f(tx + tw, ty+th);
    glVertex2f(x+width, y+height);

    glTexCoord2f(tx, ty + th);
    glVertex2f(x, y+height);

    glEnd(); //so proud :D
}
void drawEntireTexture(unsigned int geepeeyouone, float x, float y, float width, float height) {
    glBindTexture(GL_TEXTURE_2D, geepeeyouone);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex2f(x, y);

    glTexCoord2f(1, 1);
    glVertex2f(x + width, y);

    glTexCoord2f(1, 0);
    glVertex2f(x + width, y + height);

    glTexCoord2f(0, 0);
    glVertex2f(x, y + height);

    glEnd(); //so proud :D
}

#pragma endregion

#pragma region now all the lighting bites:

unsigned int textureColorBuffer;
unsigned int framebuffer;

void switchToFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, screenWidth, screenHeight);

    glClear(GL_COLOR_BUFFER_BIT);
}
void switchToScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, displayWidth, displayHeight);
}

float flashlightAngle = 0;
float defaultDarkness = 0.9;
void circle(float x, float y, float radius, int segments = 32) {
    float angle = 0;
    float angleChange = (3.1416 * 2) / segments;


    glBegin(GL_TRIANGLE_FAN);

    
    glVertex2f(x, y);

    
    for (int i = 0; i <= segments; i++) {
        angle += angleChange;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
    
}

void perspectiveCircle(float x, float y, float radius, int segments = 32) {
    float angle = 0;
    float angleChange = (3.1416 * 2) / segments;


    glBegin(GL_TRIANGLE_FAN);


    glVertex2f(x, y);


    for (int i = 0; i <= segments; i++) {
        angle += angleChange;
        glVertex2f(x + cos(angle) * radius, y + (sin(angle) * radius)*yRatio);
    }
    glEnd();

}

void line(point start, point end, float radius, bool drawEndOne = true, bool drawEndTwo = true) {
    float angle = atan2(start.y - end.y, start.x - end.x);
    angle += 3.1416 / 2;

    point vector = point(cos(angle)*radius, sin(angle)*radius);

    glBegin(GL_QUADS);

    glVertex2f(start.x + vector.x, start.y + vector.y);
    glVertex2f(end.x + vector.x, end.y + vector.y);
    glVertex2f(end.x - vector.x, end.y - vector.y);
    glVertex2f(start.x - vector.x, start.y - vector.y);

    glEnd();

    if (drawEndOne) {
        circle(start.x, start.y, radius, 16);
    }
    if (drawEndTwo) {
        circle(end.x, end.y, radius, 16);
    }


}

void radialGradient(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, float x, float y, float radius, int segments=32) {
    float angle = 0;
    float angleChange = (3.1416 * 2) / segments;
    

    glBegin(GL_TRIANGLE_FAN);

    glColor4f(innerr, innerg, innerb, innera);
    glVertex2f(x, y);

    glColor4f(r, g, b, a);
    for (int i = 0; i <=segments; i++) {
        angle += angleChange;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
    glColor4f(1, 1, 1, 1);
}

void gradientRect(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, point p1, point p2, point p3, point p4) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);

    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);

    glColor4f(innerr, innerg, innerb, innera);

    glVertex2f(p3.x, p3.y);
    glVertex2f(p4.x, p4.y);

    glEnd();
    glColor4f(1, 1, 1, 1);
}

void radialGradientPolygon(float r, float g, float b, float a, float innerr, float innerg, float innerb, float innera, vector<point> points, float radius, bool moveWithPlayer=false) {
    float px, py;
    if (moveWithPlayer) {
        px = -playerX;
        py = -playerY;
    }
    else {
        px = 0;
        py = 0;
    }
    
    for (const point p : points) {
        
        radialGradient(r, g, b, a, innerr, innerg, innerb, innera, p.x+ px, p.y+ py, radius);
        
        
    }
    point segment1, segment2;
    point p1, p2, p3, p4;
    float angle;

        for (int i = 0; i < points.size(); i++) {
            if (i == 0) {
                segment2 = points.back();
            }
            else {
                segment2 = points[i - 1];
            }
            segment1 = points[i];
            angle = atan2(segment2.y - segment1.y, segment2.x - segment1.x);

            angle += 3.1416 / 2;

            p1.x = segment1.x + px + cos(angle) * radius;
            p1.y = segment1.y + py + sin(angle) * radius;

            p2.x = segment2.x + px + cos(angle) * radius;
            p2.y = segment2.y + py + sin(angle) * radius;

            p3.x = segment2.x + px;
            p3.y = segment2.y + py;

            p4.x = segment1.x + px;
            p4.y = segment1.y + py;

            gradientRect(r, g, b, a, innerr, innerg, innerb, innera, p1, p2, p3, p4);
        }
        glBegin(GL_POLYGON);

        glColor4f(innerr, innerg, innerb, innera);

        for (point p : points) {
            glVertex2f(p.x + px, p.y + py);
        }

        glEnd();

        glColor4f(1, 1, 1, 1);

}

void drawFlashlight(float x, float y, float angle, float widthAngle) {

    float angle1 = angle + widthAngle / 2;
    float angle2 = angle - widthAngle / 2;
    int dist = 150;
    int otherdist = 300;
    int blurRadius = 5;
    float brightness = 0;

    glBegin(GL_TRIANGLES);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle1) * dist, y + sin(angle1) * dist);
    glVertex2f(x + cos(angle2) * dist, y + sin(angle2) * dist);

    glEnd();

    glBegin(GL_POLYGON);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle1) * dist, y + sin(angle1) * dist);
    glVertex2f(x + cos(angle1 + widthAngle/2) * dist, y + sin(angle1 + widthAngle / 2) * dist);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x+cos(angle1+3.1416/2)* blurRadius, y + sin(angle1 + 3.1416 / 2) * blurRadius);

    glEnd();

    glBegin(GL_POLYGON);

    glColor4f(0, 0, 0, brightness);
    glVertex2f(x, y);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle2) * dist, y + sin(angle2) * dist);
    glVertex2f(x + cos(angle2 - widthAngle / 2) * dist, y + sin(angle2 - widthAngle / 2) * dist);
    glColor4f(0, 0, 0, defaultDarkness);
    glVertex2f(x + cos(angle2 - 3.1416 / 2) * blurRadius, y + sin(angle2 - 3.1416 / 2) * blurRadius);

    glEnd();


    glColor4f(1, 1, 1, 1);

    radialGradient(0, 0, 0, defaultDarkness, 0, 0, 0, brightness, x, y, blurRadius);

}

#pragma endregion
//now the platyer bites
#pragma region player-world interactions
point drawPlayerPoint(specialPoint p) {
    point d= projectPoint(p.rx, p.ry, p.rz);
    d.x += centerX;
    d.y += centerY;
    return d;
    
}

void drawPlayer(playerModel p, float radius) {

    

    
    point butt, leftKnee, rightKnee, head, leftElbow, rightElbow;
    head=drawPlayerPoint(p.head);
    

    butt=drawPlayerPoint(p.butt);
    leftKnee=drawPlayerPoint(p.leftKnee);


    rightKnee=drawPlayerPoint(p.rightKnee);


    leftElbow=drawPlayerPoint(p.leftElbow);
    rightElbow=drawPlayerPoint(p.rightElbow);

    line(drawPlayerPoint(p.leftFoot), leftKnee, radius);
    line(drawPlayerPoint(p.rightFoot), rightKnee, radius);

    line(leftKnee, butt, radius, false, true);
    line(rightKnee, butt, radius, false, false);

    line(butt, head, radius, false, true);

    line(head, leftElbow, radius, false, false);
    line(head, rightElbow, radius, false, false);

    
    line(drawPlayerPoint(p.leftHand), leftElbow, radius);

    line(drawPlayerPoint(p.rightHand), rightElbow, radius);

    circle(head.x, head.y-3, radius+2);
    

}

struct colisionData {
    bool colision = false;
    float yMovement = 0;
    float xMovement = 0;
    colisionData() {};
};

float movePlayer() {
    //float angle = atan2(mouseY * ratio - centerY, mouseX * ratio - centerX);
    float forward = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    float sideways = glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D);
    if (forward != 0 and sideways != 0) {
        forward *= 0.71;
        sideways *= 0.71;

    }
    if (forward < 0) {
        forward *= 0.8;
    }
    sideways *= 0.8;
    playerX += cos(cursorAngle) * forward + cos(cursorAngle - 3.1416 / 2) * sideways ;
    playerY += yRatio*(sin(cursorAngle) * forward + sin(cursorAngle - 3.1416 / 2) * sideways );
    if (forward == 0 and sideways == 0) {
        return -67;
    }
    else {
        return atan2(forward, sideways);
    }
    
    
}

#pragma endregion

#pragma region Basic Object Classes
//basic game opbject
struct sprite {
public:
    int x;
    int y;
    int textureID;
    int width;
    int height;
    sprite(int Xpos, int yPos, int textureNumber) {
        x = Xpos;
        y = yPos;
        textureID = textureNumber;
        width = getTextureWidth(textureID);
        height = getTextureHeight(textureID);
    }
    sprite() {}
};
//a radial light source that makes it less dark
struct radialLight {
    float r, g, b, a;
    float radius;
    float x, y;
    radialLight(float x, float y, float radius, float r = 0, float g = 0, float b = 0, float a = 0) : x(x), y(y), radius(radius), r(r), g(g), b(b), a(a) {}
};
//a polygonal light source for windows and doors with a builtin radial gradient system to avoid bluring
struct polygonLight {
    vector<point> points;
    float r, g, b, a;
    float radius;
    polygonLight(vector<point> points, float radius, float r = 0, float g = 0, float b = 0, float a = 0) : points(points), radius(radius), r(r), g(g), b(b), a(a) {}
};
//a recatngle hitbox for player colisions
struct rect {
    float x;
    float y;
    float width;
    float height;
};
//a Interactable object that can be picked up and activate scripts.
struct interactable {
    string tag;
    float x;
    float y;
    int textureID;
    float width;
    float height;
    float cx;
    float cy;
    float radius;
    bool allowInteractions;
    interactable(float x, float y, int textureID, int radius = 100, bool allowInteractions=true, string tag="") : x(x), y(y), textureID(textureID), radius(radius), allowInteractions(allowInteractions), tag(tag) {
		width = getTextureWidth(textureID);
		height = getTextureHeight(textureID);
        cx = x + width / 2;
        cy = y + height / 2;
        cout << cx << cy << "\n";
	}
    bool interactionCheck() {
        if (!allowInteractions) {
            return false;
        }
		float distance = sqrt(pow(playerX+centerX-cx, 2) + pow(playerY+centerY-cy, 2));
        cout << "interactable distance: " << distance << "\n";
		if (distance < radius) {
			return true;
		}
		else {
			return false;
		}
    }
    void draw() {
        drawSprite(texture, textureID, x - playerX, y - playerY, width, height);
    }

};
//stores one map of sprites and lights
struct level {
public:
    vector<sprite> sprites;
    vector<radialLight> radialLights;
    vector<polygonLight> polygonLights;
	vector<interactable> interactables;
    vector<rect> hitbox;
    level(vector<sprite> spriteVector = {}, vector<interactable> interatableVector = {}, vector<radialLight> radialLightVector = {}, vector<polygonLight> polygonLightVector = {}, vector<rect> hitboxes = {}) {
        sprites = spriteVector;
		interactables = interatableVector;
        radialLights = radialLightVector;
        polygonLights = polygonLightVector;
        hitbox = hitboxes;
    }
    
};
#pragma endregion

#pragma region sad little functions that need to be after the level class because they use it 
//this is a game logic function but it needs the structures defined first so it gets to be all alone :(
//checks if the player is interacting with an object and presing space to do so.
string checkInteraction(level map) {
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        for (int index = 0; index < map.interactables.size(); index++) {
            if (map.interactables[index].interactionCheck()) {
                return map.interactables[index].tag;
            }
        }
    }
    return "NONE";
}


//HITBOX BYTES

//checks player colision with a single rectangle and returns fix if colision is true
colisionData checkColision(rect box) {
    float boxX, boxY, boxW, boxH;
    float pX, pY;
    colisionData output;

    pX = centerX + playerHitboxOffsetX;
    pY = centerY + playerHitboxOffsetY;

    boxX = box.x - playerX - playerRadius;
    boxY = box.y - playerY - playerRadius;
    boxW = box.width + playerRadius * 2;
    boxH = box.height + playerRadius * 2;

    if (
        pX > boxX
        and
        pY > boxY
        and
        pX < boxX + boxW
        and
        pY < boxY + boxH
        ) {

        output.colision = true;
        
        float x1, x2, y1, y2;

        x1 = boxX - pX;
        x2 = boxX + boxW - pX;
        y1 = boxY - pY;
        y2 = boxY + boxH - pY;
        
        if (abs(x2) < abs(x1)) {
            x1 = x2;
        }
        if (abs(y2) < abs(y1)) {
            y1 = y2;
        }
        if (abs(y1) < abs(x1)) {
            output.yMovement = y1;
        }
        else {
            output.xMovement = x1;
        }
    }
    else {
        output.colision = false;
    }
    return output;

}
colisionData OLDcheckColision(rect box) {
    colisionData output;
    float playerHitboxX = playerX + playerHitboxOffsetX;
    float playerHitboxY = playerY + playerHitboxOffsetY; //close but not right
    if (
        playerHitboxX > box.x - playerRadius
        and
        playerHitboxY > box.y - playerRadius
        and
        playerHitboxX < box.x + box.width + playerRadius
        and
        playerHitboxY < box.y + box.height + playerRadius
        ) {
        output.colision = true;
        float xChange, yChange;
        if (abs(box.x-playerRadius - playerHitboxX) < abs(box.x + box.width + playerRadius - playerHitboxX)) {
            xChange = box.x - playerRadius - playerHitboxX;
        }
        else {
            xChange = box.x + box.width + playerRadius - playerHitboxX;
        }
        if (abs(box.y-playerRadius - playerHitboxY) < abs(box.y + box.height + playerRadius - playerHitboxY)) {
            yChange = box.y - playerRadius - playerHitboxY;
        }
        else {
            yChange = box.y + box.height + playerRadius - playerHitboxY;
        }
        
        if (abs(yChange) < abs(xChange)) {
            output.yMovement = yChange;
        }
        else {
            output.xMovement = xChange;
        }
    }
    else {
        output.colision = false;
    }
    return output;
}
//calculates all player Colisions for the map
void playerColisions(level map) {
    for (int i = 0; i < 1; i++) {
        int count = 0;
        for (rect box : map.hitbox) {
            colisionData treat = checkColision(box);
            count += treat.colision;
            if (treat.colision and false) {
                cout << "player colided with rect at position X:" << playerX << ", Y:" << playerY << "and moved by  X:" << treat.xMovement << ", Y:" << treat.yMovement;
            }
            playerX += treat.xMovement;
            playerY += treat.yMovement;
            
        }
        if (count == 0) {
            break;
        }
    }
}

#pragma endregion

#pragma region DRAW EVERYTHING HORAY
//DRAWING BITES

//draws the ground :D
void drawGround(int xPos, int yPos) {
    int offsetX, offsetY = 0;
    offsetX = -xPos % 200;
    offsetY = -yPos % 200;
    //cout << offsetY <<  '\n';
    for (int x = -1; x < 5; x++) {
        for (int y = -1; y < 3; y++) {
            drawSprite(texture, __DIRT__, x * 200 + offsetX, y * 200 + offsetY, 200, 200);
        }
    }
}
//dr4aws the hitboxes of a level
void drawAllHitboxes(level map) {
    glColor4f(1, 0.3, 0.6, 0.5);
    for (rect box : map.hitbox) {

        drawRect(box.x-playerX, box.y-playerY, box.width, box.height);
    }
    glColor4f(1, 1, 1, 1);
}
//dras all the interactable objects in a game level
void drawAllInteractables(level map) {
	for (interactable obj : map.interactables) {
		obj.draw();
	}
}
//draws a level
void drawMap(level map) {
    for (sprite obj : map.sprites) {
        drawSprite(texture, obj.textureID, obj.x-playerX, obj.y-playerY, obj.width, obj.height);
    }
    drawAllInteractables(map);
    if (drawHitboxes) {
        drawAllHitboxes(map);
    }
    
}
//draws circle light scources of the a map
void drawRadialLights(level map) {
    for (radialLight light : map.radialLights) {
        radialGradient(0, 0, 0, defaultDarkness, light.r, light.g, light.b, light.a, light.x-playerX, light.y-playerY, light.radius);
    }
}
//draws polygon light sources of a mnap,  NEEDS TO HAPPEN WITH DEFAUT COLOR MIXING SETTINGS AND SHOULDNT BE ABLE TO OVERLAP WITH OTHER POLYGONS BUT CAN
void drawPolygonLights(level map) {
    for (polygonLight l : map.polygonLights) {
        radialGradientPolygon(0, 0, 0, defaultDarkness, l.r, l.g, l.b, l.a, l.points, l.radius, true);
    }
}
//draws all the text thats onscreen
void drawDialogues(vector<dialogue>& g) {
    for (auto it = g.begin(); it != g.end(); ) {
        if (it->tick()) {
            it = g.erase(it);
        }
        else {
            it->draw();

            ++it;
        }
    }
}
//holds the entire game level data 

#pragma endregion
//this one is speshal so it can be out in the open :)
struct storage {
    vector<dialogue> dialogues = {};
    vector<level> levels;
    storage() {

    }

    void addLevel(vector<sprite> sprites = {}, vector<interactable> interactables = {}, vector<radialLight> radialLights = {}, vector<polygonLight> polygonLights = {}, vector<rect> hitbox = {}) {
        levels.push_back(level(sprites, interactables, radialLights, polygonLights, hitbox));
    }

}; 
#pragma region title screen for fun

//okay now fun title screen stuff:
int screenMode = 0; //0 is title screen, 1 is game, 2 is pause menu, 3 is settings, 4 is credits, 5 is game over (almost one of these exist yet :)

int titleBack = 0;

struct menuButton {
    int x;
    int y;
    int width;
    int height;
    string text;
    menuButton(int X, int Y, string T) : x(X), y(Y), text(T) {
        width = textWidth(text);
        height = 12;
    }
    bool tick() {
        bool hover = mouseX * ratio > x and mouseX * ratio < x + width and mouseY * ratio > y and mouseY * ratio < y + height;
        if (hover) {
            glColor4f(0.8, 0.75, 0.62, 1);
            drawText(x, y, text);
            glColor4f(1, 1, 1, 1);

        }
        else {
            drawText(x, y, text);
        }
        return (hover and newClick);
    }
};

vector<menuButton> titleButtons = {
	menuButton(182, 44, "Play The Game"),
	menuButton(182, 60, "Settings"),
	menuButton(182, 76, "Credits"),
	menuButton(182, 92, "Exit")
};

void titleScreen() {
    //this is badd bro
    if (titleBack == 0) {
        if (randBool() == 1) {
            drawSprite(texture, __TITLE1FRAME1__, 0, 0, screenWidth, screenHeight);
        }
        else {
            if (randBool() == 1) {
                drawSprite(texture, __TITLE1FRAME1__, 0, 0, screenWidth, screenHeight);
            }
            else {
                if (titleBack == 0) {
                    if (randBool() == 1) {
                        drawSprite(texture, __TITLE1FRAME2__, 0, 0, screenWidth, screenHeight);
                    }
                    else {
                        if (randBool() == 1) {
                            drawSprite(texture, __TITLE1FRAME2__, 0, 0, screenWidth, screenHeight);
                        }
                        else {
                            drawSprite(texture, __TITLE1FRAME3__, 0, 0, screenWidth, screenHeight);
                        }
                    }

                }
            }
        }

    }
    //please do NOT open

    //how do i draw text i forget

    //title V
    drawText(192, 34, "( THE GAME )");




	for (menuButton& b : titleButtons) {
        if (b.tick()) {
            //do menue actuion:
			if (b.text == "Play The Game") {
				screenMode = 1;
			}
			else if (b.text == "Settings") {
				screenMode = 3;
			}
			else if (b.text == "Credits") {
				screenMode = 4;
			}
			else if (b.text == "Exit") {
				exit(0);
			}
        }
	}

    


    drawCharacter(round(mouseX * ratio ), round(mouseY * ratio) - 11, '.');


}

#pragma endregion
//this one too
storage game;
//the slightly girthy one V
int main()
{
    #pragma region levels for the game to play or something
    //creates level one V
    game.addLevel(
        //Sprite List V
        {
            sprite(50, 0, __VERTICALROAD__),
            sprite(50, -138, __VERTICALROAD__),
            sprite(50, -273, __VERTICALROAD__),
            sprite(50, -412, __VERTICALROAD__),
            sprite(-62, 140, __HORIZONTALROAD__),
            sprite(134, 140, __HORIZONTALROAD__),
            sprite(329, 140, __HORIZONTALROAD__),
            sprite(524, 140, __HORIZONTALROAD__),
            sprite(-255, 140, __HORIZONTALROAD__)
        },
        //Interactable Object List V
        {
        },
        //Radial Light Source List V
        {
            radialLight(188, 156, 71, 0, 0, 0, 0),
            radialLight(352, 156, 92, 0, 0, 0, 0),
            radialLight(548, 152, 88, 0, 0, 0, 0),
            radialLight(-10, 158, 99, 0, 0, 0, 0),
            radialLight(-215, 157, 85, 0, 0, 0, 0),
            radialLight(65, -7, 45, 0, 0, 0, 0),
            radialLight(61, -122, 43, 0, 0, 0, 0),
            radialLight(62, -235, 44, 0, 0, 0, 0),
            radialLight(64, -340, 43, 0, 0, 0, 0)
        },
        //Polygon Light Source List V
        {
        },
        //Hitbox List V
        {
            rect(184, -390, 11, 28),
            rect(185, -320, 10, 28),
            rect(184, -251, 11, 26),
            rect(185, -181, 10, 28),
            rect(185, -118, 10, 30),
            rect(185, -46, 10, 28),
            rect(185, 21, 10, 28),
            rect(184, 92, 11, 28)
        }
    );
    

    #pragma endregion
   
    #pragma region window initialization that i will never touch again eek cory no more
    //BOring bites :(
	
    if (!glfwInit()) {
        cerr << "glfw didnt initialize becuase you are bad :(";
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(displayWidth, displayHeight, "this is THEE best game of 1463", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return - 1;
    }

    
    //-----------------------------THE-----------------------------------------------BIG----------------------------------------------------ONE :)
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(0);
    
    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    std::cout << "Maximum texture size: "
        << maxTextureSize << " x "
        << maxTextureSize << std::endl;

    

    // heh
    //more boring treats from the store:
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, displayWidth, displayHeight);  

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0, 640,      // world width
        360, 0,      // world height
        -1, 1           
    );

    glMatrixMode(GL_MODELVIEW);
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        0, screenWidth,
        screenHeight, 0,
        -1, 1
    );

    glMatrixMode(GL_MODELVIEW);

    //so basicialy only use one image cuz i dont want to type this all again :/
    //i decided to hid it cus its scary
    
    if (true) {
        int width, height, channels;
        unsigned char* pixels = stbi_load(
            "sprites.png",
            &width,
            &height,
            &channels,
            4
        );

        if (pixels)
        {
            cout << "so absicaly what ahppened is the iomage was sucesfulyt scaned by meta ai adn chatgpt and then forcxefuly shoved down the gee pee yous throught hole :)! " << width << "x" << height << endl; //get rid of slop
        }
        else
        {
            cout << "Failed\n";
        }

        textureWidth = width;
        textureHeight = height;

        

        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels
        );
        
        
    }
    //too much work for a picture bro :[
    
    calculatedTextureStorage = doTheTextureBites(textureStorage, textureWidth, textureHeight); //this gota go after the masive import i think

    initFont();

    for (character c : characters) {
        cout << c.letter << c.x * 4096 << c.widtht * 4096<<"\n";
    }


    //lighting texture stuff:

    //create a framebuffer bite :(

    //to hide it all
    if (true) {
        
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        

        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            screenWidth,
            screenHeight,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            NULL
        );

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            textureColorBuffer,
            0
        );

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #pragma endregion

    //what do you think it does? >:(
    loadSounds();

    #pragma region big loop
    while (!glfwWindowShouldClose(window) and !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        #pragma region boring event grabbing
        glfwPollEvents();



        //get all the user inputs:
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        mouseRelX = mouseX - prevMouseX;

        mouseRelY = mouseY - prevMouseY;

        prevMouseX = mouseX;

        prevMouseY = mouseY;

        cursorAngle += mouseRelX/1000;
        
        flashlightAngle = cursorAngle;

		prevMouseDown = mouseDown;
         
		mouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
		newClick = mouseDown and !prevMouseDown;


        //all done
        #pragma endregion
        //game Logic now 
        //it says somewhere else what this does so dont ask me
        
        if (screenMode == 1) {


            #pragma region super simple movement bites 
            //move the player and check for colisions
            float movementAngle = movePlayer();
            playerColisions(game.levels[0]);

            //shh
            secretPlayerX = playerX;
            secretPlayerY = playerY;

            playerX = round(playerX);
            playerY = round(playerY);

            //handle player interactions with objects:

            string playerUse = checkInteraction(game.levels[0]);
            if (playerUse != "NONE") {
                cout << "Player interaction with object Tag: " << playerUse << "\n";
            }

            #pragma endregion

            #pragma region draw the main world
            switchToScreen();

            //clear it
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //draw the stuff stores in the game class
            drawGround(playerX, playerY);

            drawMap(game.levels[0]);

            #pragma endregion

            #pragma region draw the lighting effects

            switchToFramebuffer();

            //fill the layer with light
            glClearColor(0, 0, 0, defaultDarkness);
            glClear(GL_COLOR_BUFFER_BIT);

            //put before blending change on purpose
            drawPolygonLights(game.levels[0]);

            //switch to defualt lighting blending
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_MIN);
            // :|

            //draw the player light sources
            radialGradient(0, 0, 0, defaultDarkness, 0, 0, 0, 0.2, centerX, centerY, 70);

            //rendre the flashlight at the players hand
            point right = projectPoint(player.rightHand.rx, player.rightHand.ry, player.rightHand.rz);
            drawFlashlight(centerX + right.x, centerY + right.y, flashlightAngle + ((movementAngle == -67) ? 0 : (movementAngle - 3.1416 / 2)), 0.4);

            //all the lights in the world
            drawRadialLights(game.levels[0]);

            //switch back to world bledning




            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            #pragma endregion

            #pragma region Draw the cool little player guy and some other stuff
            //special code :)
            //handles the player animation
            if (movementAngle != -67) {
                if (player.motion.step()) {
                    player.reset();
                    cout << "reset player model\n";
                }
                player.motion.apply();

                player.setAngle(flashlightAngle + movementAngle);
            }
            else {
                player.reset();
                player.motion.frameNumber = 0;
                player.motion.apply();
                player.setAngle(flashlightAngle + 3.1416 / 2);
            }


            // *anti aliasing* :D
            glColor4f(0, 0, 0, 0.5);
            drawPlayer(player, 2.6);
            perspectiveCircle(centerX, centerY + 10, 7, 16);

            //the actual player
            glColor4f(0, 0, 0, 1);
            drawPlayer(player, 2);
            glColor4f(1, 1, 1, 1);


            //switch
            switchToScreen();

            //put lighting on world
            drawEntireTexture(textureColorBuffer, 0, 0, screenWidth, screenHeight);

            //cursor
            glColor4f(1, 1, 1, 1);
            glDisable(GL_TEXTURE_2D);
            //drawRect(round(centerX + cos(cursorAngle) * 40), round(centerY + sin(cursorAngle) * 40 * yRatio), 1, 1);
            glEnable(GL_TEXTURE_2D);
            #pragma endregion
        }
        else {
            #pragma region super simple awesome title screen so happy hooray
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            titleScreen();
            #pragma endregion
        }
        #pragma region final fart

        //text
        
        
        drawDialogues(game.dialogues);



        //frameRaTe Controll V

        double currentTime = glfwGetTime();

        while (currentTime-lastFrameTime < targetFrameTime) 
        {
            currentTime = glfwGetTime();
        }
        
        lastFrameTime = currentTime;





        glfwSwapBuffers(window);

        //dont tell :)
        playerX = secretPlayerX;
        playerY = secretPlayerY;
        #pragma endregion
    }
    #pragma endregion

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
