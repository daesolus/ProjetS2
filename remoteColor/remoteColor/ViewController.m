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

- (void)viewDidLoad
{
    [super viewDidLoad];
    //hide the status bar
    [[UIApplication sharedApplication] setStatusBarHidden:true];

    // Do any additional setup after loading the view, typically from a nib.
    [self tryToConnect];
}
- (void)tryToConnect
{
    // create the NSURLRequest that will be sent as the handshake
    NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://107.170.171.251:8001"]];

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
    NSLog(@"The websocket received a message: %@", message);
    dispatch_async(dispatch_get_main_queue(), ^{
        self.view.backgroundColor = [self colorWithHexString:message];
    });
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
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (UIColor*)colorWithHexString:(NSString*)str
{
    const char* cStr = [str cStringUsingEncoding:NSASCIIStringEncoding];
    long col = strtol(cStr + 1, NULL, 16);
    unsigned char r, g, b;
    b = col & 0xFF;
    g = (col >> 8) & 0xFF;
    r = (col >> 16) & 0xFF;
    return [UIColor colorWithRed:(float)r / 255.0f green:(float)g / 255.0f blue:(float)b / 255.0f alpha:1];
}
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch* touch in event.allTouches) {
        //[touch locationInView:self.view].x;
        int baseInt = arc4random() % 16777216;
        NSString* hex = [NSString stringWithFormat:@"#%06X", baseInt];
        [self.socket send:hex];
    }
}
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch* touch in event.allTouches) {
        //[touch locationInView:self.view].x;
        int baseInt = arc4random() % 16777216;
        NSString* hex = [NSString stringWithFormat:@"#%06X", baseInt];
        [self.socket send:hex];
    }
}
@end
