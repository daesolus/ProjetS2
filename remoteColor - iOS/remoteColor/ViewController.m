//
//  ViewController.m
//  remoteColor
//
//  Created by Felix Lapalme on 2015-03-15.
//  Copyright (c) 2015 Felix Lapalme. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController
NSMutableArray *allColors;
int i;
int animationType;
int concurrentAnims;
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //initializes the wormhole
    /*
    self.wormhole = [[MMWormhole alloc] initWithApplicationGroupIdentifier:@"group.com.mutualmobile.wormhole"
                                                         optionalDirectory:@"wormhole"];
    */
    
    UISwipeGestureRecognizer *left = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(swipeLeft:)];
    left.direction = UISwipeGestureRecognizerDirectionLeft ;
    [self.view addGestureRecognizer:left];
    
    UISwipeGestureRecognizer *right = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(swipeRight:)];
    right.direction = UISwipeGestureRecognizerDirectionRight ;
    [self.view addGestureRecognizer:right];
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tap:)];
    [self.view addGestureRecognizer:tap];
    
    i = 1;
    animationType = 1;
    concurrentAnims = 0;

    // Do any additional setup after loading the view, typically from a nib.
    [self tryToConnect];
}
- (BOOL)prefersStatusBarHidden {
    return YES;
}
- (void)tryToConnect
{
    // create the NSURLRequest that will be sent as the handshake
    NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://107.170.171.251:56453"]];
    // create the socket and assign delegate
    self.socket = [PSWebSocket clientSocketWithRequest:request];
    self.socket.delegate = self;
    // open socket
    [self.socket open];
}
- (void)webSocketDidOpen:(PSWebSocket*)webSocket
{
    NSLog(@"did open");
}
- (void)webSocket:(PSWebSocket*)webSocket didReceiveMessage:(id)message
{
    if ([((NSString*)message) characterAtIndex:0] == '!') {
        return;
    }
    
    NSLog(@"The websocket received a message: %@", message);
    dispatch_async(dispatch_get_main_queue(), ^{
        allColors = [[message componentsSeparatedByString:@","] mutableCopy];
        animationType = [[allColors objectAtIndex:0] intValue];
        if(animationType == 3)
            allColors = [NSMutableArray arrayWithObjects:
                         [allColors objectAtIndex:0],
                         [allColors objectAtIndex:1],
                         nil];
        [self animateAllColors];
    });
}
-(void)animateAllColors{
    concurrentAnims++;
    
    [UIView animateWithDuration:allColors.count>1?((animationType == 2)?0:1):0.5 delay:(animationType == 2)?1:0 options:UIViewAnimationOptionAllowUserInteraction
     
                        animations:^{
                            self.view.backgroundColor =[self colorWithHexString:allColors[i]];
                        }
     
                     completion:^(BOOL  completed){
                         i++;
                         if(i == allColors.count)
                             i = 1;
                         
                         concurrentAnims--;
                         if(concurrentAnims < 1)
                             [self animateAllColors];
                     }];
}
- (void)webSocket:(PSWebSocket*)webSocket didFailWithError:(NSError*)error
{
    NSLog(@"The websocket handshake/connection failed with an error: %@", error);
    [self tryToConnect];
}
- (void)webSocket:(PSWebSocket*)webSocket didCloseWithCode:(NSInteger)code reason:(NSString*)reason wasClean:(BOOL)wasClean
{
    NSLog(@"The websocket closed with code: %@, reason: %@, wasClean: %@", @(code), reason, (wasClean) ? @"YES" : @"NO");
    [self tryToConnect];
}
- (UIColor*)colorWithHexString:(NSString*)str
{
    long col = strtol([str cStringUsingEncoding:NSASCIIStringEncoding] + 1, NULL, 16);
    unsigned char b = col & 0xFF, g = (col >> 8) & 0xFF, r = (col >> 16) & 0xFF;
    return [UIColor colorWithRed:(float)r / 255.0f green:(float)g / 255.0f blue:(float)b / 255.0f alpha:1];
}
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    NSLog(@"tap");
    [self colorScreenForEvent:event];
}
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self colorScreenForEvent:event];
}
-(void)colorScreenForEvent:(UIEvent*)event{
    /*
     for (UITouch* touch in event.allTouches) {
        UIColor *color = [UIColor colorWithHue:([touch locationInView:self.view].x/self.view.frame.size.width) saturation:([touch locationInView:self.view].y/self.view.frame.size.height) brightness:1 alpha:1];
        int numComponents = (int)CGColorGetNumberOfComponents([color CGColor]);
        if (numComponents == 4)
        {
            const CGFloat *components = CGColorGetComponents([color CGColor]);
            NSString* hex = [NSString stringWithFormat:@"#%02X%02X%02X", (int)(components[0]*255.f),(int)(components[1]*255.f),(int)(components[2]*255.f)];
            if(self.socket.readyState == PSWebSocketReadyStateOpen)
                [self.socket send:hex];
        }
    }
     */
}
- (IBAction)swipeRight:(id)sender {
    NSLog(@"right");
    if(self.socket.readyState == PSWebSocketReadyStateOpen)
        [self.socket send:@"!r"];
}

- (IBAction)swipeLeft:(id)sender {
    NSLog(@"left");
    if(self.socket.readyState == PSWebSocketReadyStateOpen)
        [self.socket send:@"!l"];
}

- (IBAction)tap:(id)sender {
    NSLog(@"taplol");
    if(self.socket.readyState == PSWebSocketReadyStateOpen)
        [self.socket send:@"!m"];
}
@end
