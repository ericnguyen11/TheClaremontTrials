#include "Player.hpp"
#include "BoxCollider.hpp"
#include "PhysicsManager.hpp"

Player::Player(){

    mTimer = Timer::Instance();

    mInput = InputManager::Instance();

    mAudio = AudioManager::Instance();

    mVisible = false;

    mAnimating = false;

    mWasHit = false;

    mGPA = 0;
    mLives = 3;

    mPlayer = new Texture("player.png");
    mPlayer->Parent(this);
    mPlayer->Pos(VEC2_ZERO);

    mMoveSpeed = 400.0f;
    mMoveBounds = Vector2(20.0f, 1000.0f);

    for(int i = 0; i < MAX_BULLETS; i++) {
        mBullets[i] = new Bullet();
    }

    AddCollider(new BoxCollider(Vector2(15.0f, 67.0f)));
    AddCollider(new BoxCollider(Vector2(15.0f, 40.0f)), Vector2(15.0f, 10.0f));
    AddCollider(new BoxCollider(Vector2(15.0f, 40.0f)), Vector2(-15.0f, 10.0f));

    mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

Player::~Player(){
    
    mTimer = NULL;
    mInput = NULL;
    mAudio = NULL;

    delete mPlayer;
    mPlayer = NULL;

    for (int i = 0; i < MAX_BULLETS; i++) {
        delete mBullets[i];
        mBullets[i] = NULL;
    }
}

void Player::HandleMovement(){
    if(mInput->KeyDown(SDL_SCANCODE_RIGHT)){
        Translate(VEC2_RIGHT*mMoveSpeed*mTimer->DeltaTime(), world);
    }
    else if(mInput->KeyDown(SDL_SCANCODE_LEFT)){
        Translate(-VEC2_RIGHT*mMoveSpeed*mTimer->DeltaTime(), world);
    }

    Vector2 pos = Pos(local);

    if(pos.x < mMoveBounds.x)
        pos.x = mMoveBounds.x;
        
    else if(pos.x > mMoveBounds.y){
        pos.x = mMoveBounds.y;
    }

    Pos(pos);
}

void Player::HandleFiring() {
    if(mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
        for(int i = 0; i < MAX_BULLETS; i++) {
            if(!mBullets[i]->Active()) {
                mBullets[i]->Fire(Pos());
                mAudio->PlaySFX("shotgun.wav");
                break;
            }
        }
    }
}

void Player::Visible(bool visible){
    mVisible = visible;
}

void Player::Hit(PhysEntity* other) {
    mLives--;
    mAnimating = true;
    mWasHit = true;
}

bool Player::WasHit() {
    return mWasHit;
}

bool Player::IsAnimating(){
    return mAnimating;
}

int Player::GPA(){
    return mGPA;
}

int Player::Lives(){
    return mLives;
}

void Player::AddGPA(int change){
    mGPA += change;
}

void Player::Update(){
    if (mAnimating){

        if(mWasHit) {
            mWasHit = false;
        }

    }
    else{
        if(Active()){
            HandleMovement();
            HandleFiring();
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        mBullets[i]->Update();
    }
}

void Player::Render(){
    if (mVisible){
        if(mAnimating){

        }
        else{
            mPlayer->Render();
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        mBullets[i]->Render();
    }
}