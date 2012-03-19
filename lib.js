(function () {

	var Core = function () {

		var updates = [];

		this.print = function (that) {
			native.print(that);
		}

		this.init = function (w, h, full) {
            native.init(w, h, full);
		}

		this.exit = function () {
            native.exit();
		}

		this.run = function () {
            return native.run();
		}

		this.getRenderer = function () {
			return {

				'native' : native.getRenderer(),

				setScene : function (scn) {
					this.native.setScene(scn.native);
				},

				render : function () {
					this.native.render();
				}
			};
        }



        this.getInputDevice = function () {

            return {
				'native' : native.getInputDevice(),

                getKey : function (key) {
                    return this.native.getKey(key);
                },
                getKeyUp : function (key) {
                    return this.native.getKeyUp(key);
                },
                getKeyDown : function (key) {
                    return this.native.getKeyDown(key);
                }
			};

        }



        this.getTimer = function () {
            return {
				'native':native.getTimer(),

				deltaTime : function () {
					return this.native.deltaTime();
				}
			};
        }

        this.Scene = function () {
            this.native = native.newScene();
            
            this.addSprite = function (that) {
                this.native.addSprite(that.native);
            }

            this.free = function () {
                this.native.free();
                this.native = undefined;
            }
        }

        this.Sprite = function (image) {
            this.native = native.newSprite(image.native);

            this.addFrame = function (that) {
                this.native.addFrame(that);
            }

            this.move = function (x, y) {
				
                if (this.body == undefined) {
					this.native.move(x, y);
				} else {
					var pos = this.body.getPosition();
					pos.x += x / this.pworld.pinm;
					pos.y += y / this.pworld.pinm;
					this.body.setPosition(pos);
				}
            }

            this.rotate = function (a, x, y) {
                this.native.rotate(a, x, y);
            }

            this.show = function () {
                this.native.show();
            }

            this.hide = function () {
                 this.native.hide();
            }

            this.setSpeed = function (speed) {
                nativeSprite.setSpeed(speed);
            }
            
            this.getPosition = function () {
				return this.native.getPosition();
			}
			this.setPosition = function (x, y) {
				if (this.body == undefined) {
					this.native.setPosition(x, y);
				} else {
					this.body.setPosition(x / this.pworld.pinm, y / this.pworld.pinm);
				}
			}
			
			this.setPhysics = function (def, world) {
				if (this.body != undefined)return undefined;
				this.pworld = world;
				var pos = this.native.getPosition();
				pos.x /= world.pinm;
				pos.y /= world.pinm;
				def.position.x = pos.x;
				def.position.y = pos.y;
				if (def.shape.type == "none") {
					def.shape.type = 0;//box
					def.shape.x = (image.getW()/2)/world.pinm;
					def.shape.y = (image.getH()/2)/world.pinm;
				}
				this.body = world.native.createBody(def);
				this.update = function () {
						var pos = this.body.getPosition();
		
						this.native.setPosition ({
							x : (pos.x * world.pinm) - image.getW()/2,
							y : (pos.y * world.pinm) - image.getH()/2
						});
						
						var a = this.body.getAngle();
						this.native.rotate(a - this.native.getAngle(), image.getW()/2, image.getH()/2);
				}
				world.updates.push(this);			
			}
        }

        this.Image = function (filename) {
            this.native = native.newImage(filename);

            this.free = function () {
                this.native.free();
            }
            
            this.getW = function () {
				return this.native.getW();
			}
			
			this.getH = function() {
				return this.native.getH();
			}
        }
        
        this.PhysicsDef = function () {
			this.position = {
				x : 0,
				y : 0
			};
			this.type = 0;//0 - static, 1 - dynamic
			this.mass = 1.0;
			this.friction = 0.0;
			this.density = 0.3;
			
			this.shape = {
				type : "none", // none - auto calculate, 0 - box
				x : 1,
				y : 1
			};
		}
		
		this.createPhysicsWorld = function (x, y, pinm) {
			var w = native.createPhysicsWorld(x, y);
			res = {
				"pinm":pinm,
				"native":w,
				updates:[],
				step:function (x, y, z) {
					this.native.step(x, y, z);
					this.updates.forEach(function (x){x.update();}); 
				}		
			};
			return res;
		}
		
				

	}

	return new Core();

})();
