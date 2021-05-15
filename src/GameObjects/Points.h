#pragma once
#include "Entity.h"
#include "World.h"

class Points : public Entity
{ 
public:
	Points(std::unique_ptr<Model> model);
	void Draw(std::shared_ptr<Shader> shader) override;

private:

};