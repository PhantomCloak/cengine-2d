#include "animation_designer.h"
#include "../assetmgr/AssetManager.h"
#include "../io/cursor.h"
#include "../log/log.h"
#include "imgui.h"
#include <algorithm>
#include <list>
#include <numeric>


enum PivotDirection {
    Up,
    Right,
    Left,
    Down
};

struct Node {
    std::list<std::shared_ptr<Node>> links;
    glm::vec2 Position;
    glm::vec2 Size;
    std::string nodeName;
    PivotDirection pivotDirection;
    int animSelection;
    bool splineActive;
    int splineDirection;
    bool active;

    Node() {
    }
};


bool AnimationDesigner::IsFocused() {
    return isFocused;
}

bool GetNodeDirection(const glm::vec2& p1, const glm::vec2& p2) {
    glm::vec2 direction = p2 - p1;
    float angleRadians = std::atan2(direction.y, direction.x);
    float angleDegrees = glm::degrees(angleRadians);

    if (angleDegrees > -90 && angleDegrees < 90) {
        return true;
    } else {
        return false;
    }
}

glm::vec2 GetSpatialMidpoint(const glm::vec2& p1, const glm::vec2& cp1, const glm::vec2& cp2, const glm::vec2& p2) {
    float totalLength = 0.0f;
    glm::vec2 prevPoint = p1;
    int segments = 1000;
    for (int i = 1; i <= segments; i++) {
        float t = i / (float)segments;
        glm::vec2 currentPoint = (1 - t) * (1 - t) * (1 - t) * p1 +
                                 3 * (1 - t) * (1 - t) * t * cp1 +
                                 3 * (1 - t) * t * t * cp2 +
                                 t * t * t * p2;
        totalLength += glm::length(currentPoint - prevPoint);
        prevPoint = currentPoint;
    }

    float halfLength = totalLength / 2.0f;
    float accumulatedLength = 0.0f;
    prevPoint = p1;
    for (int i = 1; i <= segments; i++) {
        float t = i / (float)segments;
        glm::vec2 currentPoint = (1 - t) * (1 - t) * (1 - t) * p1 +
                                 3 * (1 - t) * (1 - t) * t * cp1 +
                                 3 * (1 - t) * t * t * cp2 +
                                 t * t * t * p2;
        accumulatedLength += glm::length(currentPoint - prevPoint);
        if (accumulatedLength >= halfLength) {
            return currentPoint;
        }
        prevPoint = currentPoint;
    }

    return p1;
}
void DrawBezierCurve(const glm::vec2& p1, const glm::vec2& p2) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    int offset1 = 100;
    int offset2 = -100;

    int direction = GetNodeDirection(p1, p2);

    if (direction == 1) {
        offset1 = -100;
        offset2 = 100;
    }

    glm::vec2 cp1 = glm::vec2(p1.x + 0, p1.y);
    glm::vec2 cp2 = glm::vec2(p2.x - offset2, p2.y);

    drawList->AddBezierCurve(ImVec2(p1.x, p1.y), ImVec2(cp1.x, cp1.y), ImVec2(cp2.x, cp2.y), ImVec2(p2.x, p2.y), IM_COL32(255, 255, 255, 255), 1.0f);

    glm::vec2 midPoint2 = GetSpatialMidpoint(p1, cp1, cp2, p2);
    float circleRadius = 20.0f;

    static int folderIcon = AssetManager::GetTexture("gear");

#if RENDER_BACKEND_OPENGL
    drawList->AddImage((void*)folderIcon, ImVec2(midPoint2.x - circleRadius, midPoint2.y - circleRadius), ImVec2(midPoint2.x + circleRadius, midPoint2.y + circleRadius));
#elif RENDER_BACKEND_RAYLIB
    drawList->AddImage((void*)&folderIcon, ImVec2(midPoint2.x - circleRadius, midPoint2.y - circleRadius), ImVec2(midPoint2.x + circleRadius, midPoint2.y + circleRadius));
#endif

    glm::vec2 directionVec = glm::normalize(3.0f * (p2 - cp2));
    
    float arrowLength = 20.0f;
    float arrowWidth = 10.0f;

    glm::vec2 arrowPoint1 = p2 - directionVec * arrowLength + glm::vec2(-directionVec.y, directionVec.x) * arrowWidth;
    glm::vec2 arrowPoint2 = p2 - directionVec * arrowLength - glm::vec2(-directionVec.y, directionVec.x) * arrowWidth;

    drawList->AddTriangleFilled(ImVec2(p2.x, p2.y), ImVec2(arrowPoint1.x, arrowPoint1.y), ImVec2(arrowPoint2.x, arrowPoint2.y), IM_COL32(255, 255, 255, 255));
}


int nodeCount = 0;
std::vector<std::shared_ptr<Node>> nodes;

void AnimationDesigner::AddNode(std::string nodeName) {
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->nodeName = nodeName;
    node->active = true;
    node->Size = glm::vec2(100, 50);

    nodes.emplace_back(std::move(node));
}

void DrawConnectNode(std::shared_ptr<Node>& node1) {
    for (auto& link : node1->links) {
        glm::vec2 nodePos = node1->Position;
        glm::vec2 linkPos = link->Position;

        nodePos.y += node1->Size.y / 2;
        linkPos.y += link->Size.y / 2;

        if (GetNodeDirection(nodePos, linkPos) == 1) {
            nodePos.x += node1->Size.x;
        } else {
            linkPos.x += link->Size.x;
        }

        DrawBezierCurve(nodePos, linkPos);
    }
}

void DrawConnectCursor(std::shared_ptr<Node>& node1, glm::vec2 cursorPos) {
    DrawBezierCurve(node1->Position, cursorPos);
}

void AnimationDesigner::RenderNodes() {
    for (auto& node : nodes) {
        ImGui::Begin(node->nodeName.c_str(), &node->active, ImGuiWindowFlags_NoResize);
        ImGui::Spacing();
        ImGui::Combo("Type", &node->animSelection, "Run\0Walk\0Idle\0");
        ImGui::SetWindowSize(ImVec2(node->Size.x, node->Size.y));

        ImVec2 pos = ImGui::GetWindowPos();
        node->Position = glm::vec2(pos.x, pos.y);

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Add transition")) {
                node->splineActive = true;
            }
            if (ImGui::MenuItem("Delete")) {
                node->active = false;
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        if (node->links.size() != 0) {
            DrawConnectNode(node);
        }
        if (node->splineActive) {
            glm::vec2 nodePos = node->Position;
            nodePos.y += node->Size.y / 2;

            if (GetNodeDirection(nodePos, ViewportCursorPos) == 1) {
                nodePos.x += node->Size.x;
            }
            DrawBezierCurve(nodePos, ViewportCursorPos);
            int collidingIndex = -1;
            if (Cursor::HasLeftCursorClicked()) {
                for (auto& collisionNode : nodes) {
                    if (collisionNode->Position.x < ViewportCursorPos.x && collisionNode->Position.x + collisionNode->Size.x > ViewportCursorPos.x) {
                        if (collisionNode->Position.y < ViewportCursorPos.y && collisionNode->Position.y + collisionNode->Size.y > ViewportCursorPos.y) {
                            collidingIndex = std::distance(nodes.begin(), std::find(nodes.begin(), nodes.end(), collisionNode));
                        }
                    }
                }

                if (collidingIndex != -1) {
                    node->links.push_back(nodes[collidingIndex]);
                    node->splineActive = false;
                } else {
                    node->splineActive = false;
                }
            }
        }
    }

    nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](const std::shared_ptr<Node>& node) {
        return !node->active;
    }),
    nodes.end());
}

void AnimationDesigner::RenderWindow() {
    static bool Open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Animation Editor [idle.anim]", &Open, ImGuiWindowFlags_NoNav)) {

        isFocused = ImGui::IsWindowHovered();
        static float zoomValue = 1;
        static float lastZoomValue = 1;

        auto rect = ImGui::GetContentRegionAvail();
        glm::vec2 currentRect = glm::vec2(rect.x, rect.y);

        static ImVec2 windowSize = ImVec2(0, 0);
        static ImVec2 contentAvail = ImVec2(0, 0);
        ImVec2 windowPos = ImVec2(0, 0);

        if (ViewportSize.x != currentRect.x || ViewportSize.y != currentRect.y) {
            ViewportSize = glm::vec2(currentRect.x, currentRect.y);


            windowSize = ImGui::GetWindowSize();
            contentAvail = ImGui::GetContentRegionAvail();
        }

        static bool a = true;
        RenderNodes();

        glm::vec2 pos = Cursor::GetCursorPosition();
        windowPos = ImGui::GetWindowPos();

        // pos.x -= (windowSize.x - contentAvail.x);
        // pos.y -= windowPos.x;

        // pos.y -= (windowSize.y - contentAvail.y);
        // pos.y -= windowPos.y;

        ViewportCursorPos = pos;

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("New Node")) {
                Log::Inf("New Node");
                AddNode("Node: " + std::to_string(nodeCount++));
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}
