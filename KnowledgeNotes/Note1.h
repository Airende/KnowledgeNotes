//
//  Note1.h
//  KnowledgeNotes
//
//  Created by airende on 2017/2/5.
//  Copyright © 2017年 airende. All rights reserved.
//

【题目1】找出两个 UIView 的最近的公共 View，如果不存在，则输出 nil 。
场景：暂未想到
思路：循环找父视图，对比名称看是否相同。

'思路1':将两个视图所有的 superView 分别放两个数组，然后拿其中一个数组的元素开始依次比较。 N平方
'思路2':将两个视图的父视图依次放两个 NSSet 中 每次都进行比较  N次

'思路3':最简单（不懂）
extension UIView {
    func commonSuperview(of view: UIView) -> UIView? {
        return superview.flatMap {
            view.isDescendant(of: $0) ?
            $0 : $0.commonSuperview(of: view)
        }
    }
}



【题目2】什么时候block中不需要使用 WeakSelf
场景：很多block代码中是有使用，一般都加上。
解答：block中使用weakSelf是防止循环引用问题，目前的想法是凡是自己写的都用weakself，第三方没有特别说明不需要weakSelf的都适用weakSelf。具体原理不懂。

'解答'： animateWithDuration:animations 不需要 当动画结束时，UIView 会结束持有这个 block，如果没有别的对象持有 block 的话，block 对象就会释放掉，从而 block 会释放掉对于 self 的持有。整个内存引用关系被解除。

【问题3】我们知道，从 Storyboard 往编译器拖出来的 UI 控件的属性是 weak 的，如下所示
@property (weak, nonatomic) IBOutlet UIButton *myButton;
那么，如果有一些 UI 控件我们要用代码的方式来创建，那么它应该用 weak 还是 strong 呢？为什么？

场景：很多
解答：目前的话自己代码创建的view都是使用 strong 没有出现什么问题，我了解到的是用 weak 也是可以的。

'解答'：这道题并没有标准答案，但是答案背后的解释却非常有价值，能够看出一个人对于引用计数，对于 view 的生命周期的理解是否到位。

    我认为 UI 控件用默认用 weak，根源还是苹果希望只有这些 UI 控件的父 View 来强引用它们，而 ViewController 只需要强引用 ViewController.view 成员，则可以间接持有所有的 UI 控件。这样有一个好处是：在以前，当系统收到 Memory Warning 时，会触发 ViewController 的 viewDidUnload 方法，这样的弱引用方式，可以让整个 view 整体都得到释放，也更方便重建时整体重新构造。

    所以在这种情况下，其实 UI 控件是不是 weak 其实关系并不大。当 UI 控件是 weak 时，它的引用计数是 1，持有它的是它的 superview，当 UI 控件是 strong 时，它的引用计数是 2，持有它的有两个地方，一个是它的 superview，另一个是这个 strong 的指针。UI 控件并不会持有别的对象，所以，不管是手写代码还是 Storyboard，UI 控件是 strong 都不会有循环引用的。

'【递归】'
对于一个标准的递归程序，应该有两部分组成：
    第一部分，也是最重要的部分，退出条件。
    第二部分，循环调用自身的代码。

    N! = N*(N-1)*(N-2)*...1，另外 0 的阶乘是 1，负数没有阶乘。
    int fact(int n)
    {
        if (n == 0) {
            return 1;
        } else {
            return n * fact(n–1);
        }
    }



























