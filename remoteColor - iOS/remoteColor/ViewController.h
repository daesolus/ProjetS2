//
//  ViewController.h
//  remoteColor
//
//  Created by Felix Lapalme on 2015-03-15.
//  Copyright (c) 2015 Felix Lapalme. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <PocketSocket/PSWebSocket.h>

@interface ViewController : UIViewController <PSWebSocketDelegate>

@property (nonatomic, strong) PSWebSocket *socket;

@end

