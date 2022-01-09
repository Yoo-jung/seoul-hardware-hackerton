from tkinter import *
from serial import Serial

# 내용 삭제
def delete(item) :
    print("delete 함수 실행했읍니다")
    global items
    value = listbox.get(items.index(item)) # listbox 내에서 entry_get 값을 반환
    ind = items.index(value) # items 리스트 내의 value 값 위치
    listbox.delete(items.index(item)) # 리스트 박스에서 삭제하기
    del items[ind] # times 리스트에서 [ind] 위치에 있는 값 삭제
    with open(r"C:\2022_1_Seoul-Hardware-Hackathon\GUI\text.txt", "w", encoding="UTF-8") as f: # 리스트 값 파일에 추가
        for item in items:
            f.write(item)
    main()

# 내용 생성
def add(item) :
    print("add 함수 실행했읍니다")
    global items
    items.append(item) # 엔트리 입력창에서 얻은 값 itmes 리스트 끝에 추가
    with open(r"C:\2022_1_Seoul-Hardware-Hackathon\GUI\text.txt", "w", encoding="UTF-8") as f: # 리스트 값 파일에 추가
        for item in items:
            f.write(item)
    listbox.insert(END, item) # 마지막으로 들어간 문장 밑에 추가
    main()

# 중복 체크
def check(item) :
    print("check 함수 실행했읍니다")
    global items
    if item in items : # items 리스트 안에 entry.get() 값이 있으면
        delete(item) # delete() 함수 실행
    else : # 아니면
        add(item) # add() 함수 실행

def uart() :
    if ser.readable():
        res = ser.readline()
        item = (res.decode()[:len(res)-1])
        print(item)
        check(item)
    else :
        main()

def main() :
    print("main 000000")
    window.after(10, uart)
    window.mainloop() # 윈도우 창 보이기

window = Tk() # 윈도우 생성
window.title("2022_Seoul_Hardware_Hackathon") # 윈도우 타이틀 설정
window.geometry("500x300") # 윈도우 크기 설정
window.resizable(False, False)

ser = Serial('COM3', 115200)
items = [] # 리스트 박스 내의 내용이 들어 갈 공백 리스트 생성

# 리스트 박스 생성
listbox = Listbox(window, height = 0, width = 300, selectmode = "extended") # Listbox 생성, selectmode에 대한 설명은 밑에 참고
# 리스트 박스에 데이터 추가
for i in range(len(items)): # 아이템 자료 개수만큼 반복하는데
    listbox.insert(END, items[i]) # 리스트 박스 끝에 자료 추가
listbox.pack() # 리스트 박스 보이기

if __name__ == "__main__" :
    main()
