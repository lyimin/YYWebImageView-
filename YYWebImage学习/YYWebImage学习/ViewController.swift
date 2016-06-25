//
//  ViewController.swift
//  YYWebImage学习
//
//  Created by 梁亦明 on 16/5/31.
//  Copyright © 2016年 xiaoming. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var imageview: UIImageView!
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        let url = "http://avatar.csdn.net/4/6/C/1_tuyaloveios.jpg"
        imageview.yy_setImageWithURL(NSURL(string:url), options: .UseNSURLCache)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

