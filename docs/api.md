### 数据结构规范
## 定义
# uuid
- type: string
- description: 事件的uuid
- example: "123e4567-e89b-12d3-a456-426614174000"
# time
- type: number
- description: 事件的时间戳
- example: 1610000000
# action
- type: string
- description: 按键动作
- example: "down"/"up"
# value
- type: number
- description: 按键值
- example: VK_SPACE
# friendly
- type: string
- description: 友好名称
- example: LShift
# posX/posY
- type: number
- description: 鼠标位置
- example: 0
# data
- type: text(base64)
- description: 截屏数据，jpeg格式，以base64编码
- example: 略