//
//  Note2.h
//  KnowledgeNotes
//
//  Created by airende on 2017/2/5.
//  Copyright © 2017年 airende. All rights reserved.
//

【1】'去除输入框空格(NSString也适用)
    去除两端空格
    NSString *temp = [textField.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];

    去除两端空格和回车
    NSString *text = [temp stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet ]];

【2】'返回到主线程进行操作,对UI进行更新只能在主线程进行
    /*将数据显示到UI控件,注意只能在主线程中更新UI,
     另外performSelectorOnMainThread方法是NSObject的分类方法，每个NSObject对象都有此方法，
     它调用的selector方法是当前调用控件的方法，例如使用UIImageView调用的时候selector就是UIImageView的方法
     Object：代表调用方法的参数,不过只能传递一个参数(如果有多个参数请使用对象进行封装)
     waitUntilDone:是否线程任务完成执行
     */

    [self performSelectorOnMainThread:@selector(updateImage:) withObject:data waitUntilDone:YES];

【3】'当改动布局要更新效果时
    CGRect headFrame=self.headerView.frame;
    headFrame.size.height=200;
    self.headerView.frame = headFrame;
    [self.headerView setNeedsLayout];
    [self.headerView layoutIfNeeded];

【4】'系统提供的dispatch方法
    为了方便地使用GCD，苹果提供了一些方法方便我们将block放在主线程 或 后台线程执行，或者延后执行。使用的例子如下：
    //  后台执行：
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        // something
    });
    // 主线程执行：
    dispatch_async(dispatch_get_main_queue(), ^{
        // something
    });
    // 一次性执行：
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        // code to be executed once
    });
    // 延迟2秒执行：
    double delayInSeconds = 2.0;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        // code to be executed on the main queue after delay
    });

    dispatch_queue_t 也可以自己定义，如要要自定义queue，可以用dispatch_queue_create方法，示例如下：
    dispatch_queue_t urls_queue = dispatch_queue_create("blog.devtang.com", NULL);
    dispatch_async(urls_queue, ^{
        // your code
    });
    dispatch_release(urls_queue);

    另外，GCD还有一些高级用法，例如让后台2个线程并行执行，然后等2个线程都结束后，再汇总执行结果。这个可以用dispatch_group, dispatch_group_async 和 dispatch_group_notify来实现，示例如下：
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_async(group, dispatch_get_global_queue(0,0), ^{
        // 并行执行的线程一
    });
    dispatch_group_async(group, dispatch_get_global_queue(0,0), ^{
        // 并行执行的线程二
    });
    dispatch_group_notify(group, dispatch_get_global_queue(0,0), ^{
        // 汇总结果
    });

【5】'iOS中的生成随机数方法

    生成0-x之间的随机正整数
    int value =arc4random_uniform(x ＋ 1);

    生成随机正整数
    int value = arc4random()

    通过arc4random() 获取0到x-1之间的整数的代码如下：
    int value = arc4random() % x;

    获取1到x之间的整数的代码如下:
    int value = (arc4random() % x) + 1;

    最后如果想生成一个浮点数，可以在项目中定义如下宏：
    #define ARC4RANDOM_MAX      0x100000000

    然后就可以使用arc4random() 来获取0到100之间浮点数了（精度是rand（）的两倍），代码如下：
    double val = floorf(((double)arc4random() / ARC4RANDOM_MAX) * 100.0f);

    实例(从数组中随机显示出一个背景图,再通过网络加载显示出来):
    self.bgView=[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)];
    self.bgView.image=[UIImage imageNamed:@"AppBg"];
    [self.view addSubview:self.bgView];
    [self.view sendSubviewToBack:self.bgView];

    NSDictionary *params=[[NSDictionary alloc] init];
    [[HomeMainNetAPIManager sharedManager] getBackgroundImage:params andBlock:^(id data, NSError *error) {
        if (!error&&data) {
            BackgroundImageBean *groundImagebean =(BackgroundImageBean *)data;
            int dataNum=groundImagebean.data.count;
            if (groundImagebean.data&&dataNum>0) {
                int r=arc4random_uniform(dataNum);
                GroundImageBean *curBean=groundImagebean.data[r];
                [self.bgView sd_setImageWithURL:[NSURL URLWithString:curBean.ImgUrl] placeholderImage:[UIImage imageNamed:@"AppBg"] completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        self.bgView.image=image;
                    });
                }];
            }
            
        }
    }];
【6】'沙盒路径知识整理

    模拟器的路径从之前的~/Library/Application Support/iPhone Simulator移动到了~/Library/Developer/CoreSimulator/Devices/

    文件都在个人用户名文件夹下的一个隐藏文件夹里，中文叫资源库，他的目录其实是Library。

    因为应用是在沙箱（sandbox）中的，在文件读写权限上受到限制，只能在几个目录下读写文件：
    Documents：应用中用户数据可以放在这里，iTunes备份和恢复的时候会包括此目录
    tmp：存放临时文件，iTunes不会备份和恢复此目录，此目录下文件可能会在应用退出后删除
    Library/Caches：存放缓存文件，iTunes不会备份此目录，此目录下文件不会在应用退出删除

    iTunes在与iPhone同步时，备份所有的Documents和Library文件。
    iPhone在重启时，会丢弃所有的tmp文件。

    查看方法：
    方法1、可以设置显示隐藏文件，然后在Finder下直接打开。设置查看隐藏文件的方法如下：打开终端，输入命名
    (1)显示Mac隐藏文件的命令：defaults write com.apple.finder AppleShowAllFiles -bool true
    (2)隐藏Mac隐藏文件的命令：defaults write com.apple.finder AppleShowAllFiles -bool false
    (3)输完单击Enter键，退出终端，重新启动Finder就可以了 重启Finder：鼠标单击窗口左上角的苹果标志-->强制退出-->Finder-->
    现在能看到资源库文件夹了。
    打开资源库后找到/Application Support/iPhone Simulator/文件夹。这里面就是模拟器的各个程序的沙盒目录了。
    方法2、这种方法更方便，在Finder上点->前往->前往文件夹，输入/Users/username/Library/Application Support/iPhone Simulator/  前往。
    username这里写用户名。

    自定义类返回各目录路径：

    #import <Foundation/Foundation.h>

    @interface ICSandboxHelper : NSObject

    + (NSString *)homePath;     // 程序主目录，可见子目录(3个):Documents、Library、tmp
    + (NSString *)appPath;        // 程序目录，不能存任何东西
    + (NSString *)docPath;        // 文档目录，需要ITUNES同步备份的数据存这里，可存放用户数据
    + (NSString *)libPrefPath;    // 配置目录，配置文件存这里
    + (NSString *)libCachePath;    // 缓存目录，系统永远不会删除这里的文件，ITUNES会删除
    + (NSString *)tmpPath;        // 临时缓存目录，APP退出后，系统可能会删除这里的内容
    + (BOOL)hasLive:(NSString *)path; //判断目录是否存在，不存在则创建

    实现代码:

    #import "ICSandboxHelper.h"

    @implementation ICSandboxHelper

    + (NSString *)homePath{
        return NSHomeDirectory();
    }

    + (NSString *)appPath
    {
        NSArray * paths = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSUserDomainMask, YES);
        return [paths objectAtIndex:0];
    }

    + (NSString *)docPath
    {
        NSArray * paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        return [paths objectAtIndex:0];
    }

    + (NSString *)libPrefPath
    {
        NSArray * paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
        return [[paths objectAtIndex:0] stringByAppendingFormat:@"/Preference"];
    }

    + (NSString *)libCachePath
    {
        NSArray * paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
        return [[paths objectAtIndex:0] stringByAppendingFormat:@"/Caches"];
    }

    + (NSString *)tmpPath
    {return [NSHomeDirectory() stringByAppendingFormat:@"/tmp"];
    }

    + (BOOL)hasLive:(NSString *)path
    {
        if ( NO == [[NSFileManager defaultManager] fileExistsAtPath:path] )
        {
            return [[NSFileManager defaultManager] createDirectoryAtPath:path
                                             withIntermediateDirectories:YES
                                                              attributes:nil
                                                                   error:NULL];
        }
        
        return NO;
    }
