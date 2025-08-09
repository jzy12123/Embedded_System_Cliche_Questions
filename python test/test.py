import math
import random
import sys
import time

import pygame


#############################
# 配置与常量
#############################
WINDOW_WIDTH = 960
WINDOW_HEIGHT = 600
GRID_SIZE = 30
GRID_COLS = WINDOW_WIDTH // GRID_SIZE
GRID_ROWS = WINDOW_HEIGHT // GRID_SIZE

BACKGROUND_TOP_COLOR = (12, 16, 28)
BACKGROUND_BOTTOM_COLOR = (18, 24, 44)

SNAKE_PRIMARY_COLOR = (76, 201, 240)
SNAKE_SECONDARY_COLOR = (52, 152, 219)
SNAKE_OUTLINE_COLOR = (18, 35, 57)
SNAKE_GLOW_COLOR = (76, 201, 240)

FOOD_CORE_COLOR = (255, 96, 96)
FOOD_EDGE_COLOR = (255, 180, 100)
FOOD_GLOW_COLOR = (255, 120, 120)

UI_NEON_COLOR = (0, 255, 200)
WHITE = (255, 255, 255)
GRAY = (180, 190, 210)
LIGHT_GRAY = (210, 220, 235)
RED = (255, 80, 100)

STATE_MENU = 0
STATE_RUNNING = 1
STATE_PAUSED = 2
STATE_GAME_OVER = 3


#############################
# 工具函数：图形与效果
#############################
def lerp(a: float, b: float, t: float) -> float:
    return a + (b - a) * t


def lerp_color(c1, c2, t):
    return (
        int(lerp(c1[0], c2[0], t)),
        int(lerp(c1[1], c2[1], t)),
        int(lerp(c1[2], c2[2], t)),
    )


def create_vertical_gradient(size, top_color, bottom_color):
    width, height = size
    surf = pygame.Surface(size).convert()
    for y in range(height):
        t = y / max(1, height - 1)
        color = lerp_color(top_color, bottom_color, t)
        pygame.draw.line(surf, color, (0, y), (width, y))
    return surf


def draw_glow(surface, center, base_color, max_radius, layers=4):
    for i in range(layers):
        t = i / max(1, layers - 1)
        radius = int(lerp(max_radius, 2, t))
        alpha = int(lerp(18, 80, 1 - t))
        glow = pygame.Surface((radius * 2, radius * 2), pygame.SRCALPHA)
        pygame.draw.circle(glow, (*base_color, alpha), (radius, radius), radius)
        surface.blit(glow, (center[0] - radius, center[1] - radius), special_flags=pygame.BLEND_PREMULTIPLIED)


def draw_capsule(surface, rect, color_a, color_b, outline_color=None, glow_color=None):
    x, y, w, h = rect
    radius = min(w, h) // 2
    temp = pygame.Surface((w, h), pygame.SRCALPHA)

    for i in range(h):
        t = i / max(1, h - 1)
        row_color = lerp_color(color_a, color_b, t)
        pygame.draw.rect(temp, row_color, (radius, i, w - 2 * radius, 1))

    pygame.draw.circle(temp, color_a, (radius, radius), radius)
    pygame.draw.circle(temp, color_b, (w - radius, h - radius), radius)

    # 高光
    highlight = pygame.Surface((w, h), pygame.SRCALPHA)
    pygame.draw.ellipse(highlight, (255, 255, 255, 40), (w * 0.15, h * 0.15, w * 0.7, h * 0.45))
    temp.blit(highlight, (0, 0), special_flags=pygame.BLEND_PREMULTIPLIED)

    if outline_color is not None:
        pygame.draw.circle(temp, outline_color, (radius, radius), radius, 2)
        pygame.draw.circle(temp, outline_color, (w - radius, h - radius), radius, 2)
        pygame.draw.rect(temp, outline_color, (radius, 1, w - 2 * radius, h - 2), 2, border_radius=radius)

    surface.blit(temp, (x, y))

    if glow_color is not None:
        center = (x + w // 2, y + h // 2)
        draw_glow(surface, center, glow_color, max(w, h))


def draw_gem(surface, center, radius, time_seconds):
    # 多边形宝石动画
    points = []
    sides = 8
    pulse = 1.0 + 0.08 * math.sin(time_seconds * 3)
    for i in range(sides):
        ang = (math.tau / sides) * i + time_seconds * 0.6
        r = radius * (0.85 + 0.15 * math.sin(ang * 2)) * pulse
        px = center[0] + math.cos(ang) * r
        py = center[1] + math.sin(ang) * r
        points.append((px, py))

    # 渐变填充
    gem = pygame.Surface((radius * 2 + 8, radius * 2 + 8), pygame.SRCALPHA)
    pygame.draw.polygon(gem, FOOD_EDGE_COLOR + (230,), [(x - center[0] + radius + 4, y - center[1] + radius + 4) for x, y in points])

    inner = pygame.Surface((radius * 2 + 8, radius * 2 + 8), pygame.SRCALPHA)
    pygame.draw.polygon(inner, FOOD_CORE_COLOR + (255,), [(x - center[0] + radius + 4, y - center[1] + radius + 4) for x, y in points])
    grad = pygame.Surface(inner.get_size(), pygame.SRCALPHA)
    for y in range(grad.get_height()):
        t = y / max(1, grad.get_height() - 1)
        col = lerp_color(FOOD_CORE_COLOR, FOOD_EDGE_COLOR, t)
        pygame.draw.line(grad, (*col, 110), (0, y), (grad.get_width(), y))
    inner.blit(grad, (0, 0), special_flags=pygame.BLEND_ADD)
    gem.blit(inner, (0, 0), special_flags=pygame.BLEND_PREMULTIPLIED)

    surface.blit(gem, (center[0] - gem.get_width() // 2, center[1] - gem.get_height() // 2))
    draw_glow(surface, center, FOOD_GLOW_COLOR, radius * 2)


def draw_snake_head(surface, grid_pos, cell_size, direction, base_color_a, base_color_b):
    x, y = grid_pos[0] * cell_size, grid_pos[1] * cell_size
    rect = (x + 2, y + 2, cell_size - 4, cell_size - 4)
    draw_capsule(surface, rect, base_color_a, base_color_b, outline_color=SNAKE_OUTLINE_COLOR, glow_color=SNAKE_GLOW_COLOR)

    # 眼睛
    eye_offset = cell_size // 5
    eye_r = max(3, cell_size // 10)
    cx = x + cell_size // 2
    cy = y + cell_size // 2
    if direction == (1, 0):  # 右
        p1 = (cx + eye_offset, cy - eye_offset)
        p2 = (cx + eye_offset, cy + eye_offset)
    elif direction == (-1, 0):  # 左
        p1 = (cx - eye_offset, cy - eye_offset)
        p2 = (cx - eye_offset, cy + eye_offset)
    elif direction == (0, -1):  # 上
        p1 = (cx - eye_offset, cy - eye_offset)
        p2 = (cx + eye_offset, cy - eye_offset)
    else:  # 下
        p1 = (cx - eye_offset, cy + eye_offset)
        p2 = (cx + eye_offset, cy + eye_offset)
    pygame.draw.circle(surface, WHITE, p1, eye_r)
    pygame.draw.circle(surface, WHITE, p2, eye_r)
    pygame.draw.circle(surface, (30, 60, 90), p1, eye_r // 2)
    pygame.draw.circle(surface, (30, 60, 90), p2, eye_r // 2)

    # 舌头（轻微动画）
    t = time.time()
    tongue_len = int(lerp(cell_size * 0.08, cell_size * 0.18, (math.sin(t * 8) + 1) / 2))
    if direction == (1, 0):
        pygame.draw.line(surface, RED, (x + cell_size - 6, cy), (x + cell_size - 6 + tongue_len, cy), 2)
    elif direction == (-1, 0):
        pygame.draw.line(surface, RED, (x + 6, cy), (x + 6 - tongue_len, cy), 2)
    elif direction == (0, -1):
        pygame.draw.line(surface, RED, (cx, y + 6), (cx, y + 6 - tongue_len), 2)
    else:
        pygame.draw.line(surface, RED, (cx, y + cell_size - 6), (cx, y + cell_size - 6 + tongue_len), 2)


def draw_snake_segment(surface, grid_pos, cell_size, color_a, color_b):
    x, y = grid_pos[0] * cell_size, grid_pos[1] * cell_size
    rect = (x + 3, y + 3, cell_size - 6, cell_size - 6)
    draw_capsule(surface, rect, color_a, color_b, outline_color=SNAKE_OUTLINE_COLOR)


#############################
# 游戏对象
#############################
class Particle:
    def __init__(self, x, y, radius, color, vx, vy, life):
        self.x = x
        self.y = y
        self.radius = radius
        self.color = color
        self.vx = vx
        self.vy = vy
        self.life = life
        self.age = 0.0

    def update(self, dt):
        self.x += self.vx * dt
        self.y += self.vy * dt
        self.age += dt

    @property
    def alive(self):
        return self.age < self.life

    def draw(self, surface):
        if not self.alive:
            return
        t = max(0.0, 1.0 - self.age / self.life)
        alpha = int(40 + 120 * t)
        color = (*self.color, alpha)
        radius = max(1, int(self.radius * (0.5 + 0.5 * t)))
        glow = pygame.Surface((radius * 2, radius * 2), pygame.SRCALPHA)
        pygame.draw.circle(glow, color, (radius, radius), radius)
        surface.blit(glow, (self.x - radius, self.y - radius), special_flags=pygame.BLEND_PREMULTIPLIED)


class ParticleSystem:
    def __init__(self):
        self.particles = []

    def emit_background(self, dt):
        # 背景缓慢漂浮粒子
        if random.random() < 0.04:
            x = random.uniform(0, WINDOW_WIDTH)
            y = WINDOW_HEIGHT + 10
            radius = random.uniform(2, 5)
            color = (140, 200, 255)
            vx = random.uniform(-10, 10)
            vy = random.uniform(-30, -10)
            life = random.uniform(9, 15)
            self.particles.append(Particle(x, y, radius, color, vx, vy, life))

    def burst(self, x, y):
        for _ in range(18):
            ang = random.uniform(0, math.tau)
            speed = random.uniform(60, 140)
            vx = math.cos(ang) * speed
            vy = math.sin(ang) * speed
            radius = random.uniform(2, 4)
            color = (255, random.randint(150, 220), random.randint(100, 160))
            life = random.uniform(0.6, 1.2)
            self.particles.append(Particle(x, y, radius, color, vx, vy, life))

    def update(self, dt):
        for p in self.pparticles_safe:
            p.update(dt)
        self.particles = [p for p in self.particles if p.alive]

    @property
    def pparticles_safe(self):
        # 轻微容错，防止在迭代中被修改
        return list(self.particles)

    def draw(self, surface):
        for p in self.particles:
            p.draw(surface)


class Snake:
    def __init__(self):
        self.segments = [(GRID_COLS // 2, GRID_ROWS // 2 + i) for i in range(3)]
        self.direction = (0, -1)
        self.pending_growth = 0
        self.alive = True

    def head(self):
        return self.segments[0]

    def set_direction(self, new_dir):
        if (new_dir[0] == -self.direction[0] and new_dir[1] == 0) or (
            new_dir[1] == -self.direction[1] and new_dir[0] == 0
        ):
            return
        self.direction = new_dir

    def move(self):
        if not self.alive:
            return
        hx, hy = self.head()
        nx, ny = hx + self.direction[0], hy + self.direction[1]

        # 撞墙
        if nx < 0 or nx >= GRID_COLS or ny < 0 or ny >= GRID_ROWS:
            self.alive = False
            return

        # 撞自己
        if (nx, ny) in self.segments:
            tail = self.segments[-1]
            moving_into_tail = (nx, ny) == tail and self.pending_growth == 0
            if not moving_into_tail:
                self.alive = False
                return

        self.segments.insert(0, (nx, ny))
        if self.pending_growth > 0:
            self.pending_growth -= 1
        else:
            self.segments.pop()

    def grow(self, amount=1):
        self.pending_growth += amount

    def draw(self, surface):
        if not self.segments:
            return
        # 头
        draw_snake_head(
            surface,
            self.segments[0],
            GRID_SIZE,
            self.direction,
            SNAKE_PRIMARY_COLOR,
            SNAKE_SECONDARY_COLOR,
        )
        # 身体
        for seg in self.segments[1:]:
            draw_snake_segment(
                surface,
                seg,
                GRID_SIZE,
                lerp_color(SNAKE_PRIMARY_COLOR, SNAKE_SECONDARY_COLOR, 0.25),
                lerp_color(SNAKE_PRIMARY_COLOR, SNAKE_SECONDARY_COLOR, 0.75),
            )


class Food:
    def __init__(self, snake_segments):
        self.position = self.random_position(snake_segments)
        self.time_offset = random.uniform(0, 1000)

    def random_position(self, snake_segments):
        occupied = set(snake_segments)
        candidates = [(x, y) for x in range(GRID_COLS) for y in range(GRID_ROWS) if (x, y) not in occupied]
        if not candidates:
            return (0, 0)
        return random.choice(candidates)

    def respawn(self, snake_segments):
        self.position = self.random_position(snake_segments)
        self.time_offset = random.uniform(0, 1000)

    def draw(self, surface):
        cx = self.position[0] * GRID_SIZE + GRID_SIZE // 2
        cy = self.position[1] * GRID_SIZE + GRID_SIZE // 2
        t = time.time() + self.time_offset
        draw_gem(surface, (cx, cy), GRID_SIZE // 2 - 4, t)


#############################
# UI 与状态
#############################
class FancyText:
    def __init__(self, font, text, base_color, glow_color):
        self.font = font
        self.text = text
        self.base_color = base_color
        self.glow_color = glow_color

    def render(self, t):
        surf = self.font.render(self.text, True, self.base_color)
        glow_layers = 4
        out = pygame.Surface((surf.get_width() + 32, surf.get_height() + 32), pygame.SRCALPHA)
        cx, cy = out.get_width() // 2, out.get_height() // 2
        offset = 2 + int(2 * math.sin(t * 2))
        for i in range(glow_layers):
            alpha = int(70 - i * 14)
            shade = pygame.Surface(surf.get_size(), pygame.SRCALPHA)
            shade.blit(surf, (0, 0))
            shade.fill((*self.glow_color, alpha), special_flags=pygame.BLEND_RGB_MIN)
            out.blit(shade, (16 - offset + i % 2, 16 + offset - (i % 2)), special_flags=pygame.BLEND_ADD)
        out.blit(surf, (16, 16))
        return out


class Game:
    def __init__(self):
        pygame.init()
        pygame.display.set_caption("贪吃蛇 - 精美版")
        self.screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        self.clock = pygame.time.Clock()
        self.state = STATE_MENU
        self.score = 0

        # 背景渐变
        self.background = create_vertical_gradient((WINDOW_WIDTH, WINDOW_HEIGHT), BACKGROUND_TOP_COLOR, BACKGROUND_BOTTOM_COLOR)

        # 字体
        self.font_large = pygame.font.SysFont("Microsoft YaHei UI", 72)
        if self.font_large is None:
            self.font_large = pygame.font.SysFont(None, 72)
        self.font_medium = pygame.font.SysFont("Microsoft YaHei UI", 34)
        self.font_small = pygame.font.SysFont("Microsoft YaHei UI", 20)

        self.title_text = FancyText(self.font_large, "贪吃蛇", WHITE, UI_NEON_COLOR)

        self.snake = Snake()
        self.food = Food(self.snake.segments)
        self.particles = ParticleSystem()

        # 控制移动节奏
        self.base_move_interval = 0.12
        self.time_since_move = 0.0

    def reset(self):
        self.snake = Snake()
        self.food = Food(self.snake.segments)
        self.score = 0
        self.time_since_move = 0.0
        self.state = STATE_RUNNING

    def handle_input(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit(0)
            if event.type == pygame.KEYDOWN:
                if self.state == STATE_MENU:
                    if event.key in (pygame.K_RETURN, pygame.K_KP_ENTER):
                        self.reset()
                    elif event.key == pygame.K_ESCAPE:
                        pygame.quit()
                        sys.exit(0)
                elif self.state == STATE_RUNNING:
                    if event.key in (pygame.K_UP, pygame.K_w):
                        self.snake.set_direction((0, -1))
                    elif event.key in (pygame.K_DOWN, pygame.K_s):
                        self.snake.set_direction((0, 1))
                    elif event.key in (pygame.K_LEFT, pygame.K_a):
                        self.snake.set_direction((-1, 0))
                    elif event.key in (pygame.K_RIGHT, pygame.K_d):
                        self.snake.set_direction((1, 0))
                    elif event.key == pygame.K_p:
                        self.state = STATE_PAUSED
                    elif event.key == pygame.K_ESCAPE:
                        self.state = STATE_MENU
                elif self.state == STATE_PAUSED:
                    if event.key in (pygame.K_p, pygame.K_ESCAPE):
                        self.state = STATE_RUNNING
                elif self.state == STATE_GAME_OVER:
                    if event.key in (pygame.K_r, pygame.K_RETURN, pygame.K_KP_ENTER):
                        self.reset()
                    elif event.key == pygame.K_ESCAPE:
                        self.state = STATE_MENU

    def update(self, dt):
        # 背景粒子持续产生
        self.particles.emit_background(dt)
        self.particles.update(dt)

        if self.state != STATE_RUNNING:
            return

        self.time_since_move += dt
        move_interval = max(0.06, self.base_move_interval * (0.98 ** (self.score // 5)))
        if self.time_since_move >= move_interval:
            self.snake.move()
            self.time_since_move = 0.0

            if not self.snake.alive:
                self.state = STATE_GAME_OVER
                return

            if self.snake.head() == self.food.position:
                self.score += 1
                self.snake.grow(1)
                self.particles.burst(
                    self.food.position[0] * GRID_SIZE + GRID_SIZE // 2,
                    self.food.position[1] * GRID_SIZE + GRID_SIZE // 2,
                )
                self.food.respawn(self.snake.segments)

    def draw_grid_overlay(self, surface):
        # 细腻网格叠加，降低存在感
        grid_color = (40, 50, 75)
        for x in range(0, WINDOW_WIDTH, GRID_SIZE):
            pygame.draw.line(surface, grid_color, (x, 0), (x, WINDOW_HEIGHT), 1)
        for y in range(0, WINDOW_HEIGHT, GRID_SIZE):
            pygame.draw.line(surface, grid_color, (0, y), (WINDOW_WIDTH, y), 1)

    def draw_hud(self, surface):
        score_text = self.font_medium.render(f"分数: {self.score}", True, LIGHT_GRAY)
        surface.blit(score_text, (16, 14))

        tip_text = self.font_small.render("方向键/WASD 控制  |  P 暂停  |  ESC 菜单", True, (170, 185, 210))
        surface.blit(tip_text, (16, 52))

    def draw_menu(self):
        t = time.time()
        self.screen.blit(self.background, (0, 0))
        self.draw_grid_overlay(self.screen)
        self.particles.draw(self.screen)

        title_surf = self.title_text.render(t)
        self.screen.blit(title_surf, ((WINDOW_WIDTH - title_surf.get_width()) // 2, 100))

        subtitle = self.font_medium.render("按 Enter 开始  |  ESC 退出", True, GRAY)
        self.screen.blit(subtitle, ((WINDOW_WIDTH - subtitle.get_width()) // 2, 220))

        # 中央演示：宝石与蛇头
        cx, cy = WINDOW_WIDTH // 2, WINDOW_HEIGHT // 2 + 20
        draw_gem(self.screen, (cx, cy), 26, t)
        draw_snake_head(self.screen, (GRID_COLS // 2 - 2, GRID_ROWS // 2 + 1), GRID_SIZE, (1, 0), SNAKE_PRIMARY_COLOR, SNAKE_SECONDARY_COLOR)

        footer = self.font_small.render("精美粒子背景 / 发光渐变蛇体 / 宝石食物", True, (160, 175, 200))
        self.screen.blit(footer, ((WINDOW_WIDTH - footer.get_width()) // 2, WINDOW_HEIGHT - 48))

    def draw_game(self):
        self.screen.blit(self.background, (0, 0))
        self.draw_grid_overlay(self.screen)
        self.food.draw(self.screen)
        self.snake.draw(self.screen)
        self.particles.draw(self.screen)
        self.draw_hud(self.screen)

    def draw_paused(self):
        self.draw_game()
        overlay = pygame.Surface((WINDOW_WIDTH, WINDOW_HEIGHT), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 120))
        self.screen.blit(overlay, (0, 0))
        text = self.font_large.render("暂停", True, WHITE)
        self.screen.blit(text, ((WINDOW_WIDTH - text.get_width()) // 2, (WINDOW_HEIGHT - text.get_height()) // 2))

    def draw_game_over(self):
        self.screen.blit(self.background, (0, 0))
        self.draw_grid_overlay(self.screen)
        self.particles.draw(self.screen)

        title = self.font_large.render("游戏结束", True, WHITE)
        self.screen.blit(title, ((WINDOW_WIDTH - title.get_width()) // 2, 120))

        score = self.font_medium.render(f"最终分数: {self.score}", True, LIGHT_GRAY)
        self.screen.blit(score, ((WINDOW_WIDTH - score.get_width()) // 2, 210))

        hint = self.font_medium.render("R 重新开始  |  ESC 返回菜单", True, GRAY)
        self.screen.blit(hint, ((WINDOW_WIDTH - hint.get_width()) // 2, 270))

    def run(self):
        while True:
            dt = self.clock.tick(60) / 1000.0
            self.handle_input()
            self.update(dt)

            if self.state == STATE_MENU:
                self.draw_menu()
            elif self.state == STATE_RUNNING:
                self.draw_game()
            elif self.state == STATE_PAUSED:
                self.draw_paused()
            elif self.state == STATE_GAME_OVER:
                self.draw_game_over()

            pygame.display.flip()


if __name__ == "__main__":
    try:
        Game().run()
    except Exception as e:
        print("发生错误:", e)
        pygame.quit()
        sys.exit(1)


