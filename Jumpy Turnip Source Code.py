#Game created by Andrew Luo and Jason Mao
#Sources:
#https://i.imgur.com/uAbpVXD.jpg
#https://i.imgur.com/MWy8Kdc.png
#https://i.imgur.com/oHMv8Do.png
#https://i.imgur.com/rDA8Jme.png
#https://i.imgur.com/0JOVf8e.png
#https://i.imgur.com/IqMlbJc.png

import simplegui
import math
import random

# Background image must be the same image dublicated
# and stitched together horizontally
image = simplegui.load_image("https://i.imgur.com/uAbpVXD.jpg")
turnip_img = simplegui.load_image("https://i.imgur.com/MWy8Kdc.png") #50 by 55
blur_turnip_img = simplegui.load_image("https://i.imgur.com/GIwvd7U.png") #50 by 55
bird_img = simplegui.load_image("https://i.imgur.com/oHMv8Do.png") #40 by 30
eagle_img = simplegui.load_image("https://i.imgur.com/rDA8Jme.png") #50 by 55
fireball_img = simplegui.load_image("https://i.imgur.com/0JOVf8e.png") #80 by 65
inova_img = simplegui.load_image("https://i.imgur.com/IqMlbJc.png") #60 by 60

#all 200 by 200
norm_img = simplegui.load_image("https://i.imgur.com/uSyvcPD.png")
math_img = simplegui.load_image("https://i.imgur.com/DroDZzj.png")
phys_img = simplegui.load_image("https://i.imgur.com/YDHSUsV.png")
chem_img = simplegui.load_image("https://i.imgur.com/CDRy3ZY.png")
bio_img = simplegui.load_image("https://i.imgur.com/32kgUIh.png")
geo_img = simplegui.load_image("https://i.imgur.com/aAy0Wxo.png")

parachute_img = simplegui.load_image("https://i.imgur.com/iR66mNj.png") #62 by 128

t_width = 50
t_height = 55

WIDTH = image.get_width()
HEIGHT = image.get_height()
background_pos = [0,HEIGHT/2]
scroll_speed = 2

frame_width = WIDTH/2
frame_height = HEIGHT

#global variables
accel = 0.25
spawn_time = 60
level = 1
obs_count = 0
spin = 0
score = 0
spawn_time_multiplier = 1
wait_timer = 140
difficulty = 0
difficulties = {0:"Normal", 1:"Expert"}
turnip_kinds = ["Normie", "Math", "Physics", "Chemistry", "Biology", "Geology"]
ability_cooldowns = {"Normie":750, "Math":1000, "Physics":200, 
                     "Chemistry":800, "Biology":800, "Geology":400}
ability_time = 0
autoshoot = False
last_mouse_position = (1,1)
timer = 0
auto_time = 10

mode = "Game Over"
selection_buttons = []

#list of in-game objects
projectiles = []
obstacles = []

def start():
    global start_button, difficulty_button
    global turnip
    
    kinds = ["Normie", "Math", "Physics", "Chemistry", "Biology", "Geology"]
    images = [norm_img, math_img, phys_img, chem_img, bio_img, geo_img]
    for i in range(len(kinds)):
        selection_buttons.append(Button([frame_width/2 - 140 + 100*(i%3), frame_height/2 - 130 + 110*(i/3)],
                                 [frame_width/2 - 50 + 100*(i%3), frame_height/2 - 40 + 110*(i/3)],
                                 "brown", "green", kinds[i], images[i]))
        
    
    turnip = Turnip([frame_width/4, HEIGHT/2], "Biology", 100, 25, 15, 500)
    start_button = Button([frame_width/2 - 100, frame_height/2 - 50],
                          [frame_width/2 + 100, frame_height/2 +20],
                          "brown", "green", "New Game", None)
    difficulty_button = Button([frame_width/2 - 100, frame_height/2 + 40],
                          [frame_width/2 + 100, frame_height/2 + 110],
                          "brown", "grey", "Difficulty", None)
    
def new_game(kind):
    global turnip, mode, level, spawn_time, projectiles, obstacles, obs_count, score, accel
    global speed_multiplier, damage_multiplier, health_multiplier, wait_timer, spawn_time_multiplier
    global speed_increaser, health_increaser, damage_increaser, timer
    wait_timer = 140
    turnip = Turnip([frame_width/4, HEIGHT/2], kind, 100, 25, 13, ability_cooldowns[kind])
    mode = "In Game"
    level = 1
    spawn_time = 60
    projectiles = []
    obstacles = []
    obs_count = 0
    speed_multiplier = 1
    health_multiplier = 1
    damage_multiplier = 1
    score = 0
    spawn_time_multiplier = 1
    accel = 0.25
    timer = 0
    
    speed_increaser = 0.1
    health_increaser = 0.2
    damage_increaser = 0.05
    
def rect_image(canvas, pos1, pos2, image, image_x, image_y):
    canvas.draw_image(image, [image_x/2, image_y/2], [image_x, image_y],
                      [(pos1[0] + pos2[0])/2, (pos1[1] + pos2[1])/2],
                      [pos2[0] - pos1[0], pos2[1] - pos1[1]])
    
    
def distance(pos1, pos2):
    dx = pos1[0] - pos2[0]
    dy = pos1[1] - pos2[1]
    return math.sqrt(dx**2 + dy**2)

#[hor speed, radius, health, damage, kind]
obstacle_types = [[4.5, 17, 18, 11, "bird"], [4.7, 17, 15, 11, "bird"], [5, 17, 12, 10, "bird"], [5.2, 17, 10, 10, "bird"],
                  [3, 25, 30, 18, "eagle"], [3.3, 25, 25, 15, "eagle"], [3.7, 25, 20, 15, "eagle"],
                  [2.25, 35, 42, 25, "fireball"], [2.5, 35, 35, 20, "fireball"]]
def spawn():
    #(self, position, velocity, radius, health, damage, kind)
    global obs_count, level
    obstacle_type = random.choice(obstacle_types)
    y_pos = frame_height/2 + 7.0/8*(random.random()-0.5)*frame_height
    
    speed = obstacle_type[0] * 0.9
    while (random.random() > 0.6):
        speed *= 1.25
    radius = obstacle_type[1]
    health = obstacle_type[2]
    damage = obstacle_type[3]
    kind = obstacle_type[4]
    
    obstacles.append(Obstacle([frame_width, y_pos], [-speed*speed_multiplier, 0], radius, 
                              health*health_multiplier, damage*damage_multiplier, kind))
    obs_count += 1
    if obs_count == 10 + 3*level/4:
        level_up()
        
def level_up():
    global speed_multiplier, health_multiplier, damage_multiplier, level, spawn_time_multiplier, obs_count, accel
    global speed_increaser, health_increaser, damage_increaser
    level += 1
    obs_count = 0
    spawn_time_multiplier *= 0.96
    speed_multiplier *= (1 + speed_increaser)
    health_multiplier *= (1 + health_increaser)
    damage_multiplier *= (1 + damage_increaser)
    speed_increaser *= 0.95
    health_increaser *= 0.95
    damage_increaser *= 0.95
    y_pos = frame_height/2 + 7.0/8*(random.random()-0.5)*frame_height
    obstacles.append(Obstacle([frame_width, y_pos], [-4, 0], 30, 10000, -15*damage_multiplier*1, "inova"))
    
    
class Button:
    def __init__(self, pos1, pos2, colour, fill_colour, kind, image):
        self.pos1 = pos1
        self.pos2 = pos2
        self.colour = colour
        self.fill_colour = fill_colour
        self.kind = kind
        self.image = image
        if self.image != None:
            self.image_dim = [image.get_width(), image.get_height()]
    def click(self, position):
        global mode, difficulty
        if (self.pos1[0] <= position[0] <= self.pos2[0]) and \
        (self.pos1[1] <= position[1] <= self.pos2[1]):
            if self.kind == "New Game":
                mode = "Selection"
            elif self.kind == "Difficulty":
                difficulty = (difficulty + 1)%2
            elif self.kind in ["Normie", "Biology", "Chemistry", "Physics", "Math", "Geology"]:
                new_game(self.kind)
    def draw(self, canvas):
        if self.image == None:
            canvas.draw_polygon(([self.pos1[0], self.pos1[1]], [self.pos1[0], self.pos2[1]], 
                                 [self.pos2[0], self.pos2[1]], [self.pos2[0], self.pos1[1]]),
                                3, self.colour, self.fill_colour)
        else:
            rect_image(canvas, self.pos1, self.pos2, self.image, self.image_dim[0], self.image_dim[1])
        
        if self.kind == "New Game":
            canvas.draw_text(self.kind, [frame_width/2 - 90, frame_height/2], 40, "Black")
        elif self.kind == "Difficulty":
            canvas.draw_text(difficulties[difficulty], 
                             [frame_width/2 - frame.get_canvas_textwidth(difficulties[difficulty], 20),
                                                        frame_height/2 + 90], 40, "Black")
        elif self.kind in ["Normie", "Biology", "Chemistry", "Physics", "Math", "Geology"]:
            canvas.draw_text(self.kind, [(self.pos1[0]+self.pos2[0])/2 - 
                                         frame.get_canvas_textwidth(self.kind, 20)/2, 
                              self.pos2[1] + 20], 20, "Black")
    
class Turnip:
    def __init__(self, position, kind, health, radius, damage, cooldown):
        self.position = position
        self.kind = kind
        self.health = health
        self.radius = radius
        self.damage = damage
        self.cooldown = cooldown
        self.jump_speed = 10
        self.velocity = 0
        self.ability = None
        self.load = 0
        self.activated = False
        self.image = turnip_img
    def draw(self, canvas):
        canvas.draw_image(self.image, [t_width/2, t_height/2], [t_width, t_height],
                          self.position, [t_width, t_height])
        if self.ability == "parachute":
            canvas.draw_image(parachute_img, [31, 64], [62, 128], 
                              [self.position[0], self.position[1] - 55 - t_height/2],
                              [62, 128])
    def jump(self):
        self.velocity = -8
    def update(self):
        global ability_time
        
        if ability_time > 0:
            
            if turnip.kind == "Chemistry":
                for obstacle in obstacles:
                    obstacle.condition = ["Slow", 400]
            elif turnip.kind == "Biology" and ability_time > 200:
                for obstacle in obstacles:
                    obstacle.condition = ["Poison", 100]
            
            ability_time -= 1
            if ability_time <= 0:
                self.end_ability()
        if self.cooldown > 0:
            self.cooldown -= 1
            
        if self.ability == "parachute" and self.velocity > 0:
            self.velocity += accel - self.velocity*0.15
        else:
            self.velocity += accel
        self.position[1] += self.velocity
        
        if self.kind == "Biology":
            self.health += 0.0095*damage_multiplier
        if self.load > 0:
            self.load -= 1
    def attack(self, position):
        if self.load > 0:
            return
        
        angle = math.atan2(self.position[1] - position[1], self.position[0] - position[0])
        if self.kind == "Math":
            if self.activated:
                kind = "Piercing"
            else:
                kind = "Normie"
            projectiles.append(Projectile([frame_width/4 + 20, self.position[1]], 
                                          [-40*math.cos(angle), -40*math.sin(angle)], 
                                          9, 1.1*self.damage*(health_multiplier**0.95), kind))
        elif self.kind == "Chemistry" and random.random() > 0.5:
            projectiles.append(Projectile([frame_width/4 + 20, self.position[1]], 
                                          [math.fabs(25*math.cos(angle)), -25*math.sin(angle)/(1.5-random.random()/2)], 
                                          12, 0.75*self.damage*(health_multiplier**0.95), self.kind))
        elif self.kind == "Normie":
            if self.activated:
                projectiles.append(Projectile([frame_width/4 + 20, self.position[1]], 
                                              [math.fabs(25*math.cos(angle)), -25*math.sin(angle)/(1.5-random.random()/2)], 
                                              10, 2.6*self.damage*(health_multiplier**0.95), self.kind))
            else:
                projectiles.append(Projectile([frame_width/4 + 20, self.position[1]], 
                                              [math.fabs(25*math.cos(angle)), -25*math.sin(angle)/(1.5-random.random()/2)], 
                                              5, 1.3*self.damage*(health_multiplier**0.95), self.kind))
            self.end_ability()
        else:
            angle = math.atan2(turnip.position[1] - position[1], turnip.position[0] - position[0])
            projectiles.append(Projectile([frame_width/4 + 20, self.position[1]], 
                                          [math.fabs(25*math.cos(angle)), -25*math.sin(angle)/(1.5-random.random()/2)], 
                                          5, self.damage*(health_multiplier**0.95), self.kind))
    def activate_ability(self):
        global ability_time, auto_time
        self.activated = True
        self.cooldown = ability_cooldowns[self.kind]
        if self.kind == "Normie":
            ability_time = 350
        elif self.kind == "Math":
            ability_time = 250
            auto_time = 4
        elif self.kind == "Physics":
            pass
        elif self.kind == "Chemistry":
            ability_time = 200
            for obstacle in obstacles:
                if obstacle.kind == "fireball":
                    obstacle.health -= 2.5*self.damage*(health_multiplier**0.95)
        elif self.kind == "Biology":
            ability_time = 350
            self.image = blur_turnip_img
        elif self.kind == "Geology":
            self.end_ability()
            for i in range(1, 26):
                projectiles.append(Projectile([i*frame_width/25, 0], 
                                          [0, 10], 
                                          7, 1.3*self.damage*(health_multiplier**0.95), self.kind))
    def end_ability(self):
        global auto_time
        self.activated = False
        if self.kind == "Math":
            auto_time = 10
        elif self.kind == "Biology":
            self.image = turnip_img
    
class Projectile:
    def __init__(self, position, velocity, radius, damage, kind):
        self.position = position
        self.velocity = velocity
        self.radius = radius
        self.damage = damage
        self.kind = kind
        self.hits = []
    def draw(self, canvas):
        pass
    def update(self):
        self.velocity[1] += accel
        self.position[0] += self.velocity[0]
        self.position[1] += self.velocity[1]
    def draw(self, canvas):
        if self.kind == "Normie":
            colour = "red"
        elif self.kind == "Piercing":
            colour = "purple"
        elif self.kind == "Physics":
            colour = "black"
        elif self.kind == "Chemistry":
            colour = "blue"
        elif self.kind == "Biology":
            colour = "#37592d"
        elif self.kind == "Geology":
            colour = "grey"
        canvas.draw_circle(self.position, self.radius, 1, colour, colour)
        
class Obstacle:
    def __init__(self, position, velocity, radius, health, damage, kind):
        self.position = position
        self.velocity = velocity
        self.radius = radius
        self.health = health
        self.init_health = health
        self.damage = damage
        self.kind = kind
        self.alive = True
        self.condition = [None, 0]
        self.phase = random.random() * 2 * math.pi
    def has_collided(self, other):
        if distance(self.position, other.position) < self.radius + other.radius:
            return True
        else:
            return False
    def update(self):
        speed_factor = 1
        if self.condition[0] != None:
            if self.condition [1] <= 0:
                self.condition[0] = None
            if self.condition[0] == "Slow":
                speed_factor = 0.7
                if self.kind == "fireball":
                    self.health -= 0.15*damage_multiplier
            elif self.condition[0] == "Poison":
                self.health -= 0.12*damage_multiplier
            self.condition[1] -= 1
        
        self.position[0] += self.velocity[0] * speed_factor
        self.position[1] += self.velocity[1]
        if self.health <= 0:
            self.velocity[1] += accel
        elif self.kind != "fireball" and self.kind != "inova":
            self.velocity[1] = math.sin(spin + self.phase)
        elif self.kind == "inova":
            self.velocity[1] = 5 * math.sin(spin + self.phase)
        
    def draw(self, canvas):
        percent_health = float(self.health)/self.init_health
        if self.condition[0] == "Poison":
            bar_colour = "#213A1A"
        elif self.condition[0] == "Slow":
            bar_colour = "blue"
        else:
            bar_colour = "green"
        if self.kind == "bird":
            canvas.draw_image(bird_img, [20, 15], [40, 30], self.position, [40, 30])
            if self.alive:
                canvas.draw_line([self.position[0] - 20, self.position[1] - 20],
                                 [self.position[0] - 20 + 40*percent_health, self.position[1] - 20],
                                  3, bar_colour)
        elif self.kind == "eagle":
            canvas.draw_image(eagle_img, [25, 27.5],[50, 55], self.position, [50, 55])
            if self.alive:
                canvas.draw_line([self.position[0] - 25, self.position[1] - 32.5],
                                 [self.position[0] - 25 + 50*percent_health, self.position[1] - 32.5],
                                  3, bar_colour)
        elif self.kind == "fireball":
            canvas.draw_image(fireball_img, [40, 32.5], [80, 65], self.position, [80, 65])
            if self.alive:
                canvas.draw_line([self.position[0] - 40, self.position[1] - 37.5],
                                 [self.position[0] - 40 + 70*percent_health, self.position[1] - 37.5],
                                  3, bar_colour)
        else:
             canvas.draw_image(inova_img, [30, 30], [60, 60], self.position, [60, 60])
        
def key_handler(key):
    global autoshoot, ability_time
    if key == simplegui.KEY_MAP["space"]:
        turnip.jump()
    if key == simplegui.KEY_MAP["w"] and turnip.kind == "Physics":
        turnip.ability = "parachute"
    if mode == "In Game" and key in [49, 50, 51, 52, 53, 54]:
         if turnip.kind != turnip_kinds[key-49]:
            ability_time = 0
            turnip.ability = None
            turnip.kind = turnip_kinds[key-49]
            turnip.load = 75
            turnip.health -= 5
            turnip.cooldown = ability_cooldowns[turnip.kind]
    if key == simplegui.KEY_MAP["a"]:
        if autoshoot:
            autoshoot = False
        else:
            autoshoot = True
    elif key == simplegui.KEY_MAP["d"] and turnip.cooldown <= 0:
        turnip.activate_ability()
        
def keyup_handler(key):
    if key == simplegui.KEY_MAP["w"]:
        turnip.ability = None
        
def mouse_handler(position):
    global last_mouse_position
    last_mouse_position = position
    position = [position[0], position[1]]
    if mode == "In Game": 
        if turnip.kind == "Physics" and turnip.activated:
            angle = math.atan2(turnip.position[1] - position[1], turnip.position[0] - position[0])
            projectiles.append(Projectile([frame_width/4 + 20, turnip.position[1]], 
                                          [math.fabs(25*math.cos(angle)), -25*math.sin(angle)/(1.5-random.random()/2)], 
                                          70, 3.5*turnip.damage*(health_multiplier**0.95), turnip.kind))
            turnip.end_ability()
        elif (autoshoot == False or turnip.kind == "Math"):
            turnip.attack(position)
    elif mode == "Game Over":
        start_button.click(position)
        difficulty_button.click(position)
    elif mode == "Selection":
        for button in selection_buttons:
            button.click(position)

# Handler to draw on canvas
def draw(canvas):
    global spawn_time, mode, spin, score, wait_timer, timer
    #background stuff
    canvas.draw_image(image,(WIDTH/2,HEIGHT/2),(WIDTH,HEIGHT), background_pos,(WIDTH,HEIGHT))
    # Update background position 
    background_pos[0] = (background_pos[0] - scroll_speed)%(WIDTH/2)
    if mode == "Game Over":
        start_button.draw(canvas)
        difficulty_button.draw(canvas)
        canvas.draw_text("Jumpy Turnip", [frame_width/2 - 110, frame_height/2-100], 40, "Black")
        canvas.draw_text("Score: " + str(int(score)), [frame_width/2 - 100, frame_height/2+150], 40, "Black")
    
    elif mode == "Selection":
        for button in selection_buttons:
            button.draw(canvas)
        
    elif mode == "In Game":
        timer += 1
        if autoshoot and timer%auto_time == 0:
            turnip.attack(last_mouse_position)
            
        turnip.draw(canvas)
        if wait_timer > 0:
            if (wait_timer/20)%2 == 0:
                canvas.draw_text("Space to jump and click to shoot", [500, frame_height-100], 30, "Black")
                if turnip.kind == "Physics":
                    canvas.draw_text("Hold w for parachute", [550, frame_height-65], 30, "Black")
            wait_timer -= 1
            return
        score += 0.1
        spin = (spin+0.05)%(12*math.pi)
        
        turnip.update()
        if turnip.position[1] < -5 or turnip.position[1] > frame_height + 5:
            if turnip.kind == "Geology":
                turnip.health -= 0.15
            elif turnip.kind == "Physics":
                turnip.health -= 0.35
            else:
                turnip.health -= 0.5
        if turnip.health <= 0:
            mode = "Game Over"
        
        #health bar, cooldown bar, level
        health_width = turnip.health*2
        canvas.draw_polygon(([10,10], [10, 30], [210, 30], [210, 10]),
                            1, 'red', 'red')
        canvas.draw_polygon(([10,10], [10, 30], [10+health_width, 30], [10+health_width, 10]),
                            1, 'green', 'green')
        
        cooldown_width = 200 * float(ability_cooldowns[turnip.kind] - turnip.cooldown)/ability_cooldowns[turnip.kind]
        canvas.draw_polygon(([10,35], [10, 55], [210, 55], [210, 35]),
                            1, 'grey', 'grey')
        canvas.draw_polygon(([10,35], [10, 55], [10+cooldown_width, 55], [10+cooldown_width, 35]),
                            1, 'blue', 'blue')
 
        canvas.draw_text("Level: " + str(level), (10, 75), 20, "black")
        canvas.draw_text("Score: " + str(int(score)), (10, 95), 20, "black")
        
        #updates all projectiles
        for projectile in projectiles:
            projectile.update()
            projectile.draw(canvas)
            if projectile.position[1] > frame_height or projectile.position[0] > frame_width:
                projectiles.remove(projectile)
         
        #timer hits 0, spawn new obstacle
        spawn_time -= 1
        if spawn_time <= 0:
            spawn()
            spawn_time = spawn_time_multiplier*(70 + random.randint(-20, 20))
            if spawn_time < 25:
                spawn()
            if spawn_time < 10:
                spawn()
            if spawn_time < 0:
                spawn()
            if spawn_time < -15:
                spawn()
            
        for obstacle in obstacles:
            obstacle.update()
            obstacle.draw(canvas)
            for projectile in projectiles:
                if obstacle.has_collided(projectile) and obstacle not in projectile.hits:
                    if projectile.kind == "Physics":
                        projectile.velocity[1] = -10
                        projectile.velocity[0] *= 0.5
                        projectile.damage *= 0.85
                        projectile.hits.append(obstacle)
                        obstacle.health -= projectile.damage
                    elif projectile.kind == "Piercing" and obstacle.alive == False:
                        pass
                    else:
                        boost = 1
                        projectiles.remove(projectile)
                        if projectile.kind == "Chemistry":
                            obstacle.condition = ["Slow", 250]
                            if obstacle.kind == "fireball":
                                boost = 1.3
                            if projectile.radius > 10:
                                x,y = projectile.position[0], projectile.position[1]
                                x_vel, y_vel = projectile.velocity[0], projectile.velocity[1]
                                for i in range(1,random.randint(2,3)):
                                    temp_projectile = Projectile([x,y], 
                                                       [x_vel*0.5, -2*i],
                                                       4, projectile.damage/2, projectile.kind)
                                    temp_projectile.hits.append(obstacle)
                                    projectiles.append(temp_projectile)
                                    
                        elif projectile.kind == "Biology":
                            obstacle.condition = ["Poison", 150]
                            if obstacle.kind != "fireball":
                                boost = 1.2
                        obstacle.health -= projectile.damage*boost
                    
            if obstacle.health <= 0:
                if obstacle.alive == True:
                    obstacle.alive = False
                    obstacle.velocity[1] = -5
                    score += health_multiplier*obstacle.damage/2
                
            if obstacle.position[0] < -400:
                if difficulty == 1:
                    obstacle.position[0] = frame_width
                else:
                    obstacles.remove(obstacle)
            elif (obstacle.position[1] > frame_height + 10 and obstacle.alive == False):
                obstacles.remove(obstacle)
            else:
                if obstacle.has_collided(turnip) and obstacle.alive:
                    if turnip.kind == "Geology" and obstacle.kind != "inova":
                        turnip.health -= obstacle.damage*0.85
                    elif turnip.kind == "Biology" and turnip.activated and \
                        (obstacle.kind == "bird" or obstacle.kind == "eagle"):
                        pass
                    elif obstacle.condition[0] == "Slow" and obstacle.kind != "inova":
                        turnip.health -= obstacle.damage*0.75
                    else:
                        turnip.health -= obstacle.damage
                    obstacles.remove(obstacle)
        
                
    
    
    
# Create a frame and assign callbacks to event handlers
frame = simplegui.create_frame("Home", WIDTH/2, HEIGHT)
frame.set_draw_handler(draw)

#event handlers
frame.set_mouseclick_handler(mouse_handler)
frame.set_keydown_handler(key_handler)
frame.set_keyup_handler(keyup_handler)

# Start the frame animation
start()
frame.start()
