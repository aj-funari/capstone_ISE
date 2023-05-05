extends KinematicBody2D

export (int) var speed = 1200
export (int) var jump_speed = -1800
export (int) var gravity = 4000
export (float, 0, 1.0) var friction = 0.1
export (float, 0, 1.0) var acceleration = 0.25

var velocity = Vector2.ZERO
var val = 0
var val2 = 0

func get_input():
	var dir = 0
	if Input.is_action_pressed("reset_scene"):
		get_tree().reload_current_scene()
	if Input.is_action_pressed("walk_right"):
		dir += 1
	if Input.is_action_pressed("walk_left"):
		dir -= 1
	if dir != 0:
		velocity.x = lerp(velocity.x, dir * speed, acceleration)
	else:
		velocity.x = lerp(velocity.x, 0, friction)

func _physics_process(delta):
	get_input()
	velocity.y += gravity * delta
	velocity = move_and_slide(velocity, Vector2.UP)
	if Input.is_action_just_pressed("jump"):
		if is_on_floor():
			velocity.y = jump_speed


func _on_Area2D_body_entered(body):
	if(val == 1):
		get_tree().reload_current_scene()
	else:
		val += 1


func _on_Pits_of_Doom_body_entered(body):
	if(val2 == 2):
		get_tree().reload_current_scene()
	else:
		val2 += 1
