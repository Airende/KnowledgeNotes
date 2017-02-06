//
//  Note2.h
//  KnowledgeNotes
//
//  Created by airende on 2017/2/5.
//  Copyright © 2017年 airende. All rights reserved.
//

iOS应用性能调优建议和技巧

【1】'如果你有不透明的Views，你应该设置它们的opaque属性为YES。

    在相对比较静止的画面中，设置这个属性不会有太大影响。然而当这个view嵌在scroll view里边，或者是一个复杂动画的一部分，不设置这个属性的话会在很大程度上影响app的性能。


【2】'选择是否缓存图片
    UIImage *img = [UIImage imageNamed:@"myImage"]; // caching
    UIImage *img = [UIImage imageWithContentsOfFile:@"myImage"]; // no caching

【3】'避免日期格式转换
    如果你可以控制你所处理的日期格式，尽量选择Unix时间戳。你可以方便地从时间戳转换到NSDate:
    - (NSDate*)dateFromUnixTimestamp:(NSTimeInterval)timestamp {
        return [NSDate dateWithTimeIntervalSince1970:timestamp];
    }

【4】'数组排序 字符串／时间。。。
    http://beyondvincent.com/2014/01/26/2014-01-26-how-to-sort-nsarray-with-custom-objects/

【5】'iOS视图控制对象生命周期（时间顺序）
    init－初始化程序
    viewDidLoad－加载视图
    viewWillAppear－UIViewController对象的视图即将加入窗口时调用；
    viewDidApper－UIViewController对象的视图已经加入到窗口时调用；
    viewWillDisappear－UIViewController对象的视图即将消失、被覆盖或是隐藏时调用；
    viewDidDisappear－UIViewController对象的视图已经消失、被覆盖或是隐藏时调用；

【6】"demo"'iOS关于在UITableView中，实现多个cell中不同的倒计时实现

//所有剩余时间数组
NSMutableArray *totalLastTime;
在网络请求到的所有数据中，根据需要将其中要进行倒计时显示的数据中的剩余时间单独保存出来，如果这里是所有的数据都需要倒计时，则只需要保存时间即可，如果是有部分数据才需要倒计时，则可以保存字典，两个键值对分别为其在UITableView的indexPath和剩余时间：num默认从0开始
NSDictionary *dic = @{@"indexPath":[NSStrin stringWithFormat:@"%i",num],@"lastTime": order.payLastTime};
[totalLastTime addObject:dic];
开启定时器方法：
- (void)startTimer
{
    timer = [NSTimer scheduledTimerWithTimeInterval:1 target:selfselector:@selector(refreshLessTime) userInfo:@"" repeats:YES];
    
    如果不添加下面这条语句，在UITableView拖动的时候，会阻塞定时器的调用
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:UITrackingRunLoopMode];
    
}

主要的定时器中的方法，在该方法中，遍历totalLastTime，取出其中保存的lasttime和indexpath，time用来显示，在显示完后自减，indexpath代表对应显示的位置，在一次循环后，将新的time和没有改变的indexpath从新替换totalLastTime 中对应位置的元素，以此保证每一秒执行时，显示time都是最新的。
- (void)refreshLessTime
{
    NSUInteger time;
    for (int i = 0; i < totalLastTime.count; i++) {
        time = [[[totalLastTime objectAtIndex:i] objectForKey:@"lastTime"]integerValue];
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:0 inSection:[[[totalLastTime objectAtIndex:i] objectForKey:@"indexPath"] integerValue]];
        WLServiceOrderTableViewCell *cell = (WLServiceOrderTableViewCell *)[_tableView cellForRowAtIndexPath:indexPath];
        cell.remainingTimeLabel.text = [NSString stringWithFormat:@"剩余支付时间：%@",[self lessSecondToDay:--time]];
        NSDictionary *dic = @{@"indexPath": [NSStringstringWithFormat:@"%i",indexPath.section],@"lastTime": [NSStringstringWithFormat:@"%i",time]};
        [totalLastTime replaceObjectAtIndex:i withObject:dic];
    }
}

- (NSString *)lessSecondToDay:(NSUInteger)seconds
{
    NSUInteger day  = (NSUInteger)seconds/(24*3600);
    NSUInteger hour = (NSUInteger)(seconds%(24*3600))/3600;
    NSUInteger min  = (NSUInteger)(seconds%(3600))/60;
    NSUInteger second = (NSUInteger)(seconds%60);
    
    NSString *time = [NSString stringWithFormat:@"%lu日%lu小时%lu分钟%lu秒",(unsigned long)day,(unsigned long)hour,(unsigned long)min,(unsigned long)second];
    return time;
    
}

//----项目中运用中的代码：
- (void)startTimer
{
    self.timer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(refreshLessTime) userInfo:@"" repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:UITrackingRunLoopMode];
    
}

- (void)refreshLessTime
{
    NSInteger time;
    for (int i = 0; i < self.totalLastTime.count; i++) {
        time = [[[self.totalLastTime objectAtIndex:i] objectForKey:@"lastTime"]integerValue];
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:[[[self.totalLastTime objectAtIndex:i] objectForKey:@"indexPath"] integerValue] inSection:0];
        BDFindActivityCell *cell = (BDFindActivityCell *)[_tableView cellForRowAtIndexPath:indexPath];
        if (time==0) {
            [cell setCellData:@"活动中"];
            cell.myLabel.text=@"正在抢单中";
            cell.clockImageView.hidden=YES;
            //            [self.tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
            --time;
        }
        else if(time>0)
        {
            cell.myLabel.text = [NSString stringWithFormat:@"%@",[self lessSecondToDay:--time]];
        }
        NSDictionary *dic = @{@"indexPath": [NSString stringWithFormat:@"%ld",indexPath.row],@"lastTime": [NSString stringWithFormat:@"%li",time]};
        [self.totalLastTime replaceObjectAtIndex:i withObject:dic];
    }
}

- (NSString *)lessSecondToDay:(NSUInteger)seconds
{
    NSUInteger hour = (NSUInteger)(seconds%(24*3600))/3600;
    NSUInteger min  = (NSUInteger)(seconds%(3600))/60;
    NSUInteger second = (NSUInteger)(seconds%60);
    
    NSString *time = [NSString stringWithFormat:@"去抢单 %02lu:%02lu:%02lu",(unsigned long)hour,(unsigned long)min,(unsigned long)second];
    return time;
    
}

【7】'IOS关于UIImageView的拉伸问题
http://images2015.cnblogs.com/blog/80291/201603/80291-20160302142053892-452255609.png

    UIImage* img=[UIImage imageNamed:@"2.png"];//原图
    UIEdgeInsets edge=UIEdgeInsetsMake(0, 10, 0,10);
    //UIImageResizingModeStretch：拉伸模式，通过拉伸UIEdgeInsets指定的矩形区域来填充图片 上下左右都会
    //UIImageResizingModeTile：平铺模式，通过重复显示UIEdgeInsets指定的矩形区域来填充图
    img= [img resizableImageWithCapInsets:edge resizingMode:UIImageResizingModeStretch];
    self.imageView.image=img;

    项目中的运用：
    self.myImageView=[[UIImageView alloc]initWithFrame:CGRectMake(0, 100, 184, 25)];

    UIImage *curImage=[UIImage imageNamed:@"form_no_selected_icon"];

    UIEdgeInsets edge=UIEdgeInsetsMake(0, 40, 0,25);

    curImage= [curImage resizableImageWithCapInsets:edge resizingMode:UIImageResizingModeStretch];

    self.myImageView.image=curImage;

    [self.view addSubview:self.myImageView];

    如果只是左右拉伸就用UIImageResizingModeStretch，关于edge的值左右自个根据实际的大小进行确定，把以这块进行拉伸，特别要注意2X跟3X的图片；

【8】'怎么改变UITextfield placeholder的颜色和位置

继承UITextfield，重写这个方法
- (void) drawPlaceholderInRect:(CGRect)rect {
    [[UIColor blueColor] setFill];
    [self.placeholder drawInRect:rect withFont:self.font lineBreakMode:UILineBreakModeTailTruncation alignment:self.textAlignment];
}






