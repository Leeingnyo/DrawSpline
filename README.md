# 2016년 봄 컴퓨터 그래픽스 수업 과제3

[과제 문서](http://mrl.snu.ac.kr/courses/CourseGraphics/2016spring_lecture/ComputerGraphics_PA_SweptSurface.pdf)

## 잘 했다고 생각하는 점
C++를 잘 활용했나..?ㅜㅜ 초반부의 구조는 되게 잘 짠 것 같다. Spline Type에 따라 Segment를 만들고 그게 점을 생성하는 역할 거기서 한다는 것은 잘 했다고 생각한다. 원하는 스플라인 타입에 따라 Segment를 만들고 계산하면 된다!

## 못 했다고 생각하는 점 / 아쉬운 점
Quaternion이 다른 것들 (vec3, float)과 스플라인을 만드는 과정이 달라서 따로 빼주었는데 코드 중복도 많고 해서 아쉽다. 템플릿을 사용하지말고 Splinable같은 클래스를 만들어서 Vector, Float, Quaternion Subclass를 만들어 처리하게 하는 방법으로 할 걸 그랬지만 시간이 없어서 그냥 만들었다. 아니면 템플릿을 고수하고 쿼터니온에 대한 wrapper class를 만들고 연산자 오버로딩을 맞춰 해주면서 segment 등의 연산을 좀 손보거나... 둘 다 전체를 버리고 여유가 없어서 안 했다.

또 출력할 때 surface로부터 자꾸 점을 생성해내서 그리는 바람에 느려지는 것 같다 (출력은 후반이라 대충 짠 감이 있다) SurfaceBuilder::Build()에서는 vecotr of vector of vec3를 Surface에 넘겨주는 것이 더 좋다고 생각된다.

근데 과제는 벌써 냈으니 상관없다

## 해야할 점
data.txt 등 지우기
