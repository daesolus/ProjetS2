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
    // Do any additional setup after loading the view, typically from a nib.
    [self tryToConnect];
}
- (BOOL)prefersStatusBarHidden {
    return YES;
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
- (UIColor*)colorWithHexString:(NSString*)str
{
    long col = strtol([str cStringUsingEncoding:NSASCIIStringEncoding] + 1, NULL, 16);
    unsigned char b = col & 0xFF, g = (col >> 8) & 0xFF, r = (col >> 16) & 0xFF;
    return [UIColor colorWithRed:(float)r / 255.0f green:(float)g / 255.0f blue:(float)b / 255.0f alpha:1];
}
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self colorScreenForEvent:event];
}
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    [self colorScreenForEvent:event];
}
-(void)colorScreenForEvent:(UIEvent*)event{
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
}
@end
